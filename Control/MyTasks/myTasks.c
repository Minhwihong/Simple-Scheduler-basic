/**
  ******************************************************************************
  * @file    FMC/FMC_NOR/main.h
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    21-October-2015
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#include <stdint.h>
#include <string.h>
#include "myTasks.h"

#include "taskDef.h"


volatile u32 arr_stack[MAX_STACK_SIZE] = {0};

u32 single_msg_que[eTID_MAX][MSG_QUEUE_LEN] = {0};
u16 single_msg_que_sts[eTID_MAX] = {0};

extern tcb_t tcb_list[eTID_MAX];
extern tcb_t tcb_super_list[eSuperId_Max];


extern tcb_t* tcb_ready_item;
extern tcb_t* tcb_prior_item;
extern tcb_t* tcb_ready_bak;

extern u8 flag_prior_task ;

task_stack_t tasks[eTID_MAX] = {
    //Idle Task is always on the top
    {eTID_IDLE,              (void*)0,  256,    (void*)0,  Task_Idle},
    {eTID_TIMER,             (void*)0,  256,    (void*)0,  Task_Timer},
    
    {eTID_MCU_LED_TEST,      (void*)0,  128,    (void*)0,  Task_MCU_LED_Test},
    {eTID_DISP_TEST,         (void*)0,  128,    (void*)0,  Task_Disp_Test},
    {eTID_GPIO_EXIT_TEST,    (void*)0,  128,    (void*)0,  Task_Gpio_Exit_Test},

    {eTID_PWM_GEN_TEST,      (void*)0,  256,    (void*)0,  Task_PWM_Gen_Test},
    {eTID_GPIO_INPUT_TEST,   (void*)0,  128,    (void*)0,  Task_GPIO_Input_Task},

    {eTID_TEMPERATURE_TEST,  (void*)0,  512,    (void*)0,  Task_Temperature_Test},

    {eTID_MCU_ADC_TEST,      (void*)0,  512,    (void*)0,  Task_MCU_ADC_Test},
    {eTID_GPIO_OUT_TEST,     (void*)0,  128,    (void*)0,  Task_GPIO_Out_Test},

    {eTID_REMOTE_CTL_TEST,   (void*)0,  512,    (void*)0,  Task_Remote_Ctl_Test},
    {eTID_EEPROM_TEST,       (void*)0,  128,    (void*)0,  Task_EEPROM_Test},
  
};



task_stack_t tasks_super[eSuperId_Max] = {
     {eSuperId_PWM_MANAGE,  (void*)0,  256,    (void*)0,  TaskSuper_PWM_Manage},
};


void Task_Init(){

    for(u8 idx=0; idx<eTID_MAX; ++idx){

        Kernel_Add_TCB(tcb_list, tasks[idx].id, &tasks[idx], T_RUN, (void*)0);
    }

    for(u8 idx=0; idx<eSuperId_Max; ++idx){

        Kernel_Add_TCB(tcb_super_list, tasks_super[idx].id, &tasks_super[idx], T_RUN, (void*)0);
    }
}


/* 
Setting Task in a TCB execution attribute. 
Note : period is not accurate but it is almost same over 10ms scale.
       It ensures a task not running within a period
*/
void TCB_Option_Init(){
    //                  Task ID                 Repeat attr.       period   Init Run state
    TCB_Exec_Option_Set(eTID_MCU_LED_TEST,     REPEAT_ENABLE,      500,     T_RUN);
    TCB_Exec_Option_Set(eTID_DISP_TEST,        REPEAT_ENABLE,      30,      T_RUN);
    TCB_Exec_Option_Set(eTID_GPIO_EXIT_TEST,   REPEAT_DISABLE,     0,       T_HOLD);

    TCB_Exec_Option_Set(eTID_PWM_GEN_TEST,      REPEAT_DISABLE,    0,       T_HOLD);
    TCB_Exec_Option_Set(eTID_GPIO_INPUT_TEST,   REPEAT_ENABLE,     0,       T_HOLD);
    TCB_Exec_Option_Set(eTID_TEMPERATURE_TEST,  REPEAT_ENABLE,     50,      T_RUN);
    TCB_Exec_Option_Set(eTID_MCU_ADC_TEST,      REPEAT_DISABLE,     0,      T_HOLD);
    TCB_Exec_Option_Set(eTID_GPIO_OUT_TEST,     REPEAT_DISABLE,     0,      T_HOLD);


    tcb_super_list[eSuperId_PWM_MANAGE].repeat = REPEAT_ENABLE;
    tcb_super_list[eSuperId_PWM_MANAGE].runflag = T_RUN;
    tcb_super_list[eSuperId_PWM_MANAGE].period = 0;
}


void TCB_Exec_Option_Set(u8 id, u8 repeat, u32 period, u8 run_flag){

  tcb_list[id].repeat       = repeat;
  tcb_list[id].runflag      = run_flag;
  tcb_list[id].period       = period;
  tcb_list[id].first_run    = 1;
}


void Task_Unblock(u8 id, u8 repeat){

    tcb_list[id].runflag = T_RUN;
    tcb_list[id].repeat = repeat;
    tcb_list[id].first_run = 1;
}


void Task_Block(u8 id){

    tcb_list[id].runflag = T_HOLD;
}

u8 Task_Get_Run_State(u8 id){



    return tcb_list[id].runflag;
}



u8 Task_Start_Ctl(u8 id){

    if(tcb_list[id].repeat == REPEAT_DISABLE){

        if(tcb_list[id].first_run > 0){
            tcb_list[id].first_run = 0;
            return 0;
        }
        else {
            tcb_list[id].runflag = T_HOLD;
            
            //Task yield
            TASK_YIELD();
            //*(u32 volatile *)0xE000ED04 |= (1U << 28);
            return 1;
        }
       
    }
    else if(tcb_list[id].repeat == REPEAT_ENABLE){

        if(tcb_list[id].period > 0 && tcb_list[id].first_run == 0){
            Kernel_Delay_Set(id, tcb_list[id].period);
        }
        else {
            tcb_list[id].first_run = 0;
        }
        
    }

    return 0;
}




signed char Active_Super_Task(unsigned char id){

    CRITICAL_SECTION_IN();

    signed char rtn = Kernel_Manage_SuperTask(PRIOR_TCB_ADD, &tcb_super_list[id]);

    if(tcb_prior_item != NULL){
        flag_prior_task = PRIOR_TASK_START;
    }

    CRITICAL_SECTION_OUT();
    TASK_YIELD();

    return rtn;
}




signed char Deactive_Super_Task(unsigned char id){

    CRITICAL_SECTION_IN();

    signed char rtn = Kernel_Manage_SuperTask(PRIOR_TCB_REMOVE, &tcb_super_list[id]);

    if(tcb_prior_item == NULL){
        flag_prior_task = PRIOR_TASK_END;
    }

    CRITICAL_SECTION_OUT();
    TASK_YIELD();

    return rtn;
}



signed char Receive_Msg_Single(unsigned char id,  u32* pm){

    unsigned short queue_sts = 0;

    if(single_msg_que_sts[id] == 0){
        return 1;
    }

    queue_sts = single_msg_que_sts[id];

    *pm =  single_msg_que[id][queue_sts-1];
    single_msg_que_sts[id] = single_msg_que_sts[id] - 1;

     return 0;
}


signed char Send_Msg_Single(unsigned char id,  u32 pm){

     unsigned short* queue_sts = (void*)0;

    if(single_msg_que_sts[id] == MSG_QUEUE_LEN || id > eTID_MAX){
        return 1;
    }

    queue_sts = &single_msg_que_sts[id];

    single_msg_que[id][*queue_sts] 			= pm;

    (*queue_sts)++;
          
    return 0;
}









