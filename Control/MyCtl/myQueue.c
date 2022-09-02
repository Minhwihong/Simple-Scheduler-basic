#include "myQueue.h"
#include "comm.h"







Comm_queue_t 	commQueue;
Comm_queue_t 	cSend_CAN_Queue;
CANFrm_queue_t CANFrmQueue;

fp_queue_t		fsmHookFnQueue;







void CommQueue_init(Comm_queue_t* queue){

	queue->front = queue->rear = 0;
	queue->start = 1;
	
}


void CommQueue_clear(Comm_queue_t* queue){

	queue->front 		= queue->rear;
	queue->start 		= 0;
	queue->use_single	= COMM_QUEUE_USE;


}


void Commuse_single_queue(Comm_queue_t* queue, uint8_t mode){


	if(mode == 1)	queue->use_single = COMM_QUEUE_NOT_USE;
	else			queue->use_single = COMM_QUEUE_USE;

	queue->front = queue->rear = 0;
	queue->start = 1;


}


int8_t CommQueue_push_back(Comm_queue_t* queue, Comm_data_t*      inPtr){


	if(queue->start == 0)	return 0;

	if(queue->use_single == COMM_QUEUE_NOT_USE)
	{
		queue->buf[0] = *inPtr;
	}
	else
	{
		if ((queue->rear + 1) % COMM_MAX_QUEUE == queue->front)
		{
			return -1;
		}

		queue->buf[queue->rear++] = *inPtr;
		queue->rear = queue->rear % COMM_MAX_QUEUE;
		queue->count++;
	}

	
	return 1;
}


Comm_data_t* CommQueue_get_data(Comm_queue_t* queue, uint8_t* valid_pkt){


	Comm_data_t* ret = NULL;
	
	
	if(queue->start == 0)	return ret;

	if(queue->use_single == COMM_QUEUE_NOT_USE)
	{
		ret 			= &queue->buf[0];
	}
	else
	{	
		if (queue->front == queue->rear)
		{
			*valid_pkt	= 0;
			return ret;
		}

		ret 			= &queue->buf[queue->front];
		*valid_pkt		= queue->count;
		queue->front++;
		queue->front = queue->front % COMM_MAX_QUEUE;
		queue->count--;
	}

	
	return ret;

}




void FP_Queue_init(fp_queue_t* queue){

	queue->front = queue->rear = 0;
	queue->start = 1;
	
}


void FP_Queue_clear(fp_queue_t* queue){

	queue->front 		= queue->rear;
	queue->start 		= 0;
	queue->use_single	= COMM_QUEUE_USE;


}


void FP_use_single_queue(fp_queue_t* queue, uint8_t mode){


	if(mode == 1)	queue->use_single = COMM_QUEUE_NOT_USE;
	else			queue->use_single = COMM_QUEUE_USE;

	queue->front = queue->rear = 0;
	queue->start = 1;


}


int8_t FP_Queue_push_back(fp_queue_t* queue, void*       inPtr){


	if(queue->start == 0)	return 0;

	if(queue->use_single == COMM_QUEUE_NOT_USE)
	{
		queue->buf[0] = inPtr;
	}
	else
	{
		if ((queue->rear + 1) % COMM_MAX_QUEUE == queue->front)
		{
			return -1;
		}

		queue->buf[queue->rear++] = inPtr;
		queue->rear = queue->rear % COMM_MAX_QUEUE;
		queue->count++;
	}

	
	return 1;
}


void* FP_Queue_get_data(fp_queue_t* queue, uint8_t* valid_pkt){


	void* ret = NULL;
	
	
	if(queue->start == 0)	return ret;

	if(queue->use_single == COMM_QUEUE_NOT_USE)
	{
		ret 			= queue->buf[0];
	}
	else
	{	
		if (queue->front == queue->rear)
		{
			*valid_pkt	= 0;
			return ret;
		}

		ret 			= queue->buf[queue->front];
		*valid_pkt		= queue->count;
		queue->front++;
		queue->front = queue->front % COMM_MAX_QUEUE;
		queue->count--;
	}

	
	return ret;

}





void CANFrameQueue_init(void){
	
	CANFrmQueue.front = CANFrmQueue.rear = 0;
//	CANFrmQueue.start = 1;
}


void CANFrameQueue_clear(void){

	CANFrmQueue.front 		= CANFrmQueue.rear;
	CANFrmQueue.start 		= 0;


}



int8_t CANFrameQueue_push_back(uint8_t*       inPtr){


	if ((CANFrmQueue.rear + 1) % 4 == CANFrmQueue.front)
	{
		return -1;
	}


	//CANFrmQueue.buf[CANFrmQueue.rear] = *inPtr;
	memcpy(&CANFrmQueue.buf[CANFrmQueue.rear][0], inPtr, sizeof(uint8_t)*8);
	
	CANFrmQueue.rear++;
	CANFrmQueue.rear = CANFrmQueue.rear % COMM_MAX_QUEUE;
	CANFrmQueue.count++;
	
	
	return 1;
}


uint8_t* CANFrameQueue_get_data(uint8_t* valid_pkt){

	uint8_t* ret = NULL;
	
	
	if (CANFrmQueue.front == CANFrmQueue.rear)
	{
		*valid_pkt	= 0;
		return ret;
	}

	ret 			= &CANFrmQueue.buf[CANFrmQueue.front][0];
	*valid_pkt		= CANFrmQueue.count;
	
	CANFrmQueue.front++;
	CANFrmQueue.front = CANFrmQueue.front % 4;
	CANFrmQueue.count--;
	
	
	return ret;

}




