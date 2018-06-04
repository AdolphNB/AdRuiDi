
#include "peripherals.h"
#include "avr/io.h"
#include <avr/interrupt.h>
#include "MSG_Queue.h"

volatile unsigned char tick = 0;
volatile unsigned char gToggleValue = 0;
extern StructParam_Def WorkStatus;
extern StructInput_flag_t InFlag;
static volatile uint32_t System_Tick = 0;
RepayDate_t CurDate;


uint32_t Get_SystemTick()
{
	return System_Tick;
}


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

	System_Tick++;
	
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

    if (CurDate.flag){}//receive screen date and time
        
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



void ReadScreen_CurrentDate(RepayDate_t *data)
{
	uint8_t Buf[14];
	
    CurDate.flag = 1;

    Buf[4]  = 0x02;Buf[5]  = 0x00;
    Buf[6]  = 0xC4;Buf[7]  = 0xDC;//
    Buf[8]  = 0xC1;Buf[9]  = 0xBF;//
    Buf[10] = 0xC9;Buf[11] = 0xE8;
    Buf[12] = 0xD6;Buf[13] = 0xC3;
    SendToMonitor(Buf,14);

	while(CurDate.flag);
	CurDate.year = 18;
	CurDate.month = 6;
	CurDate.day = 2;	
}





TimeoutTask_t TimeoutTask = {
	.flag = 0,
};



void StartTimeout_Task(uint8_t pic, uint16_t dly)
{
	TimeoutTask.flag =  1;
	TimeoutTask.pic = pic;
	TimeoutTask.period = dly;
	TimeoutTask.timeStamp = Get_SystemTick();
}



void DestroyTimeout_Task()
{
	memset(&TimeoutTask, 0, sizeof(TimeoutTask_t));
}



void TimeoutTask_PutToQueue()
{
	uint32_t systime = Get_SystemTick();
	MSG_BufferTypeDef q;


	if(TimeoutTask.flag){

		if((systime - TimeoutTask.timeStamp) >= TimeoutTask.period){

			q.pic = TimeoutTask.pic;
			q.c = 0xff;
			MSG_QueuePut(&q);

			TimeoutTask.flag = 0;
		}
		
	}
}



