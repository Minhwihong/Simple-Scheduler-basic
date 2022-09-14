  /**
  ******************************************************************************
  * @file     Template/main.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-January-2014
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MY_TASK_DEF_H__
#define __MY_TASK_DEF_H__

#include "ModelConfig.h"
#include "port_gpio.h"
#include "main.h"


#define TEST_ITEM_MAX   11

#define TICK_TEMP_UNIT	50
#define TICK_DISP_UNIT	20

#define NO_MSG_SYMBOL   0xFFF0
#define TASK_INIT_SYMBOL    0xFFFF





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




void TCB_Option_Init();



void Task_Idle(u32 param);
void Task_Timer(u32 param);

void Task_MCU_LED_Test(u32 param);
void Task_Disp_Test(u32 param);
void Task_Gpio_Exit_Test(u32 param);

void Task_PWM_Gen_Test(u32 param);
void Task_GPIO_Input_Task(u32 param);
void Task_Temperature_Test(u32 param);
void Task_MCU_ADC_Test(u32 param);
void Task_GPIO_Out_Test(u32 param);
void Task_EEPROM_Test(u32 param);
void Task_Remote_Ctl_Test(u32 param);

void TaskSuper_PWM_Manage(u32 param);




#endif /* __MESSAGE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
