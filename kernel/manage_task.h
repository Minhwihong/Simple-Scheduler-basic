#ifndef __MANAGE_TASK_H__
#define __MANAGE_TASK_H__

#include "ModelConfig.h"
#include "kernel.h"


#define MSG_QUEUE_LEN   1

#define CURR_TASK_ID    (tcb_curr->task->id)

enum {
	T_RUN = 1,
    T_HOLD,
    T_KILLED,
    T_SUPER,
};




enum {
    REPEAT_ENABLE = 1,
    REPEAT_DISABLE
};



enum {
    T_ERR_TASK = -1,
    T_BLANK,
    T_RESTART_TASK,
    T_NEW_TASK,
    T_STOP_TASK,
    T_RUNNING_TASK,
    T_GOOD_TASK
};


void Task_Init(task_stack_t* tasks, task_stack_t* tasks_super );

u8 Task_Start_Ctl(u8 id);

void TCB_Exec_Option_Set(u8 id, u8 repeat, u32 period, u8 run_flag);
void Task_Unblock(u8 id, u8 repeat);
void Task_Block(u8 id);


u8 Task_Get_Run_State(u8 id);


signed char Active_Super_Task(unsigned char id);
signed char Deactive_Super_Task(unsigned char id);



signed char Send_Msg_Single(unsigned char id,  u32 pm);
signed char Receive_Msg_Single(unsigned char id,  u32* msg);



#endif