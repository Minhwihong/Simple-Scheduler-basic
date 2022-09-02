
#ifndef TASK_H
#define	TASK_H

#include "main.h"
#include "ModelConfig.h"
#include "kernel.h"

#define TICK_TEMP_UNIT	50
#define TICK_DISP_UNIT	20

#define MSG_QUEUE_LEN   1

#define NO_MSG_SYMBOL   0xFFF0
#define TASK_INIT_SYMBOL    0xFFFF


#define CURR_TASK_ID    (tcb_curr->task->id)

enum eTask_id {
    eID_IDLE = 0,
    eId_TIMER,
    eID_MCU_LED_TEST,
    eID_DISP_TEST,
    eID_GPIO_EXIT_TEST,
    eID_PWM_GEN_TEST,
    eID_GPIO_INPUT_TEST,
    eID_TEMPERATURE_TEST,
    eID_MCU_ADC_TEST,
    eID_GPIO_OUT_TEST,
    eId_Max,
    eId_UNVALID_TASK = 0xFF
};


enum eTask_super_id{
    eSuperId_PWM_MANAGE = 0,
    eSuperId_Max
};






void Task_Init();
void TCB_Option_Init();
u8 Task_Start_Ctl(uint8_t id);


void TCB_Repeat_Option_Set(uint8_t id, uint8_t repeat, uint32_t period, uint8_t run_flag);
void Task_Unblock(uint8_t id, uint8_t repeat);
void Task_Block(uint8_t id);
uint8_t Task_Get_Run_State(uint8_t);


signed char Active_Super_Task(unsigned char id);
signed char Deactive_Super_Task(unsigned char id);



signed char Send_Msg_Single(unsigned char id,  uint32_t pm);
signed char Receive_Msg_Single(unsigned char id,  uint32_t* msg);



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





#endif	/* TASK_H */

