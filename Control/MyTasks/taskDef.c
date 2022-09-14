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
#include "manage_task.h"
#include "taskDef.h"


volatile u32 arr_stack[MAX_STACK_SIZE] = {0};


extern tcb_t tcb_list[eTID_MAX];
extern tcb_t tcb_super_list[eSuperId_Max];


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










