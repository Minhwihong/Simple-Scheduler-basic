
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
    eTID_IDLE = 0,
    eTID_TIMER,
    eTID_MCU_LED_TEST,
    eTID_DISP_TEST,
    eTID_GPIO_EXIT_TEST,
    eTID_PWM_GEN_TEST,
    eTID_GPIO_INPUT_TEST,
    eTID_TEMPERATURE_TEST,
    eTID_MCU_ADC_TEST,
    eTID_GPIO_OUT_TEST,
    eTID_REMOTE_CTL_TEST,
    eTID_EEPROM_TEST,
    eTID_MAX,
    eId_UNVALID_TASK = (u8)0xFF
};


enum eTask_super_id{
    eSuperId_PWM_MANAGE = 0,
    eSuperId_Max
};






void Task_Init();
void TCB_Option_Init();
u8 Task_Start_Ctl(u8 id);


void TCB_Exec_Option_Set(u8 id, u8 repeat, u32 period, u8 run_flag);
void Task_Unblock(u8 id, u8 repeat);
void Task_Block(u8 id);
u8 Task_Get_Run_State(u8);


signed char Active_Super_Task(unsigned char id);
signed char Deactive_Super_Task(unsigned char id);



signed char Send_Msg_Single(unsigned char id,  u32 pm);
signed char Receive_Msg_Single(unsigned char id,  u32* msg);



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

