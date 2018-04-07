
#include "MSG_Queue.h"
#include <avr/interrupt.h>


MSG_QueueTypeDef MSG_Queue;

void MSG_QueueInit(void)
{
	MSG_QueueTypeDef *q = &MSG_Queue;
	q->first = q->last = 0;
}

int MSG_QueuePut(MSG_BufferTypeDef *d)
{
	MSG_QueueTypeDef *q = &MSG_Queue;

    // overflow ???
    if (q->last >= 0xfffffffe) {
            q->last -= q->first;
            q->first = 0;
    }

    // full ?
    if (q->last - q->first >= MSG_QUEUE_LEN)
            q->first++;
	cli();
    memcpy(&(q->data[q->last % MSG_QUEUE_LEN]), d, sizeof(MSG_BufferTypeDef));
    q->last++;
	sei(); 
    return 0;
}

int MSG_QueueGet(MSG_BufferTypeDef *value)
{
	MSG_QueueTypeDef *q = &MSG_Queue;

    if (q->first < q->last) {
		memcpy(value, &(q->data[q->first % MSG_QUEUE_LEN]), sizeof(MSG_BufferTypeDef));
		q->first++;
		return 0;
    }
    return -1;
}

int MSG_QueueIsEmpty(void)
{
	MSG_QueueTypeDef *q = &MSG_Queue;

	return (q->last > q->first)? 0 : 1;
}




