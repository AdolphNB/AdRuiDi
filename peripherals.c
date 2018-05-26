
#include "peripherals.h"
#include "avr/io.h"
#include <avr/interrupt.h>
#include "MSG_Queue.h"

volatile unsigned char tick = 0;
volatile unsigned char gToggleValue = 0;
extern StructParam_Def WorkStatus;
extern StructInput_flag_t InFlag;

void key_scan()
{
	MSG_BufferTypeDef q_input;
	uint8_t val = GET_PD_INPUT_VALUE();

	if((val & ECG_INPUT_IO)){
		//q_input.c = 0x10;
		MSG_QueuePut(&q_input);
	}else if((val & TEMPERATURE_INPUT_IO)){
		//q_input.c = 0x48;
		//MSG_QueuePut(&q_input);
	}else if((val & MANUAL_INPUT_IO)){
		//q_input.c = 0x10;
		//MSG_QueuePut(&q_input);
	}
}

ISR(TIMER1_COMPA_vect)
{
	MSG_BufferTypeDef q_tim;
    if(tick++ > gToggleValue){
		//q_tim.c = 0x10;
		q_tim.c = MSG_LCD_COUNTER_SHOW;
		if(WorkStatus.trg == TROGGLE_TING)
			MSG_QueuePut(&q_tim);
		tick = 0;
	}else if(tick > 250){
		tick = 0;
	}
}
volatile uint8_t usart_cnt = 0,usart_len;
ISR(USART0_RX_vect)
{
	uint8_t temp;
	static MSG_BufferTypeDef usart_q;

	
	temp = UDR0;
	
	switch(usart_cnt)
	{

	
		case 0:
			
			if(temp == 0x5a) 
				usart_cnt++;
			else usart_cnt = 0;
			
			break;

			
		case 1:
			
			if(temp == 0xa5) 
				usart_cnt++;
			else usart_cnt = 0;
			
			break;

			
		case 2:
			
			usart_len = temp + 2;
			usart_cnt++;
			
			break;

			
		default:
						
			if(usart_cnt == (usart_len - 1)){
				
				usart_cnt++;
				usart_q.pic = temp;
				
			}else if(usart_cnt == usart_len){
			
				usart_q.c = temp;
				MSG_QueuePut(&usart_q);
				usart_cnt = 0;
				
			}else if(usart_cnt++ > 14){
			
				usart_cnt = 0;

			}
			
			break;
		
	}
}

void USART_Transmit( unsigned char data )
{
	while ( !( UCSR0A & (1<<UDRE0)));
	UDR0= data;
}

uint8_t DevInit_TickTimer(void)
{
	cli();
	TCCR1B &= ~0x07;
	TCCR1B |= 0x03;//64 prescaler clock
	TCCR1A &= ~0x03;	//normal model
	TCCR1B &= ~(0x03<<3);//normal model, can't connect external IO
	TCCR1B |=  0x01<<3;//use CTC model
	TCNT1=0;
	OCR1A = CFG_TIMER1_INTERRUPT_INTERVAL;//clear 0,when TCNT1 enqule to this value
	TIMSK  &= ~(0xf0<<2);
	TIMSK  |= (0x01<<4);//when TCNT1 match wich OCR1A, interrupt 
	ETIMSK &= ~0x01;
	TIFR   |= 1<<2;//flag be set.when tcnt1 to thd top
	SFIOR  &= 0x7e;  //open the prescaler clock
	sei();  
	return 0;
}




uint8_t DevInit_PWMOut(void)
{
	TCCR3A = 0x83;
	TCCR3B = 0X0A;//0x15;
	TCNT3  = 0x0000;
	OCR3A  = 500;//g_energy_PWM_array[3];
	TCCR3C = 0X00;

	TIMSK=0X00;
 	ETIMSK=0X00;

	PORTE |= 0Xff;
	DDRE  |= 0Xff;
	return 0;
}

uint8_t DevInit_OutputIO(void)
{
	
	return 0;	
}



uint8_t DevInit_InputIO(void)
{
	DDRD &=  0X8F;//~((1 << PD4) | (1 << PD5) | (1 << PD6));
 	PORTD |= 0X70;//0111 0000
	return 0;
}



uint8_t USART_Init(void)
{
	cli();
	PORTE |= (1 << PE0);
	DDRE  &= ~( 1<< PE0);
	
	PORTE &= ~(1 << PE1);
	DDRE  |= (1 << PE1);

	UBRR0H = 0x00;  
	UBRR0L = 0x03;  //115200 baud
	UCSR0A = UCSR0A & 0x02;  
	UCSR0B = (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);  
	UCSR1C = (1<<UCSZ01) | (1<<UCSZ02);  
	sei();   
	return 0;
}

