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
#include "myTasks.h"
#include "port_gpio.h"
#include "main.h"
#include "kernel.h"


#define TEST_ITEM_MAX   11


typedef struct tag_test_item{
    char* test_name;
    u8 test_id;
    struct tag_test_item* next_item;
    struct tag_test_item* prev_item;
}test_item_t;


enum e_test_item{
    
    eTEST_PWM_DMA = 0,
    eTEST_CLOSE_SENS,
    eTEST_TEMPERATURE,
    eTEST_REMOTE_COMM,
    eTEST_EEPROM_RW,
    eTEST_GPIO_CTL,
    eTEST_ADC_CHECK,
    eTEST_MAX
};

extern tcb_t* tcb_curr;



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


u8 Get_TaskId_From_TestItem(u8 test_id);








s8 Program_Words_MCU_Flash(u32 baseAddr, u32 offset, u8* data);
s8 Erase_MCU_Flash(u32 startSec, u32 NbOfSec);
u32 GetSector(u32 Address);


void Console_port_init();
void Console_Rx_Cb();

void Init_test_item_list();


void Buzzor_Start(u16 tick);
void Buzzor_Operate();

#endif /* __MESSAGE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
