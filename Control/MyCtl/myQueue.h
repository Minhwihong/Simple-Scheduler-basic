#ifndef __MY_QUEUE_H__
#define __MY_QUEUE_H__


#include <comm.h>
#include "main.h"

typedef struct tag_Comm_queue
{
	uint8_t			front;
	uint8_t			rear;
	uint8_t			start;
	uint8_t			use_single;
	int8_t			count;
	Comm_data_t 		buf[COMM_MAX_QUEUE];
}Comm_queue_t;


typedef struct tag_CANFrm_queue
{
	uint8_t			front;
	uint8_t			rear;
	uint8_t			start;
	uint8_t			use_single;
	int8_t			count;
	uint8_t			buf[8][8];
	//DispBtn_data_t 	buf[3];
}CANFrm_queue_t;



typedef struct tag_fsmFp_queue {
	uint8_t			front;
	uint8_t			rear;
	uint8_t			start;
	uint8_t			use_single;
	int8_t			count;
	void* 			buf[5];

}fp_queue_t;




extern Comm_queue_t 	commQueue;
//extern Dbg_queue_t     CANFrmQueue;
extern Comm_queue_t 	cSend_CAN_Queue;
extern fp_queue_t		fsmHookFnQueue;


void CommQueue_init(Comm_queue_t* queue);
void CommQueue_clear(Comm_queue_t* queue);
void Commuse_single_queue(Comm_queue_t* queue, uint8_t mode);
int8_t CommQueue_push_back(Comm_queue_t* queue, Comm_data_t*      inPtr);
Comm_data_t* CommQueue_get_data(Comm_queue_t* queue, uint8_t* valid_pkt);




void FP_Queue_init(fp_queue_t* queue);
void FP_Queue_clear(fp_queue_t* queue);
void FP_use_single_queue(fp_queue_t* queue, uint8_t mode);
int8_t FP_Queue_push_back(fp_queue_t* queue, void*       inPtr);
void* FP_Queue_get_data(fp_queue_t* queue, uint8_t* valid_pkt);



void CANFrameQueue_init(void);
void CANFrameQueue_clear(void);
int8_t CANFrameQueue_push_back(uint8_t*    inPtr);
uint8_t* CANFrameQueue_get_data(uint8_t* valid_pkt);

#endif




