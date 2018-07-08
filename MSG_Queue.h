
#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

#include "string.h"
#include "config.h"


#define MSG_QUEUE_LEN 128


/******************************************************/
#define MSG_LCD_COUNTER_SHOW		0X10

/******************************************************/
#define MSG_CTRL_TOGGLE_CLOSE	0X32
#define MSG_CTRL_TOGGLE_OPEN	0X33


#define MSG_CTRL_KV_OPEN			0X31
#define MSG_CTRL_KV_CLOSE			0X30

#define MSG_CTRL_WATERFLOODING_OPEN 0X95
#define MSG_CTRL_WATERFLOODING_STOP 0X96

#define MSG_CTRL_DEWATER_OPEN		0X97
#define MSG_CTRL_DEWATER_STOP		0X98


#define MSG_CTRL_ADD_WATER_OPEN		0X91
#define MSG_CTRL_ADD_WATER_STOP		0X92

#define MSG_CTRL_DEC_WATER_OPEN		0X93
#define MSG_CTRL_DEC_WATER_STOP		0X94

#define MSG_CTRL_ALERT_OPEN			0X48
#define MSG_CTRL_ALERT_CLOSE		0X49

#define MSG_CTRL_ENERGY_OPEN		0X4A
#define MSG_CTRL_ENERGY_CLOSE		0X4B

/******************************************************/
#define MSG_PARAM_SET_ADD			0X23
#define MSG_PARAM_SET_DEC			0X24

/******************************************************/
#define MSG_SHOW_ENERGY_SET			0X20
#define MSG_SHOW_FREQUENCY_SET		0X21
#define MSG_SHOW_TIMES_SET			0X22
#define MSG_SHOW_CLEAR_SET			0X25





typedef struct {
	unsigned char pic;
	unsigned char c;
}MSG_BufferTypeDef;

typedef struct {
        long int first, last;
        MSG_BufferTypeDef data[MSG_QUEUE_LEN];
} MSG_QueueTypeDef;




void MSG_QueueInit(void);
int MSG_QueuePut(MSG_BufferTypeDef *d);
int MSG_QueueGet(MSG_BufferTypeDef *value);
int MSG_QueueIsEmpty(void);

#endif //__MSG_QUEUE_H__


