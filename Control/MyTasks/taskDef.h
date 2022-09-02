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
    
    eTest_cover_open = 0,
    eTest_cover_lock,
    eTest_limit_sens,
    eTest_temperature,
    eTest_Induction,
    eTest_CAN_comm,
    eTest_MP3_player,
    eTest_EEPROM_RW,
    eTest_valve,
    eTest_oil_pump,
    eTest_pressure,
    eTest_fan,
    e_test_Max
};

extern tcb_t* tcb_curr;



void Task_Idle(uint32_t param);
void Task_Timer(uint32_t param);

void Task_MCU_LED_Test(uint32_t param);
void Task_Disp_Test(uint32_t param);
void Task_Gpio_Exit_Test(uint32_t param);

void Task_PWM_Gen_Test(uint32_t param);
void Task_GPIO_Input_Task(uint32_t param);
void Task_Temperature_Test(uint32_t param);
void Task_MCU_ADC_Test(uint32_t param);
void Task_GPIO_Out_Test(uint32_t param);
void Task_Fan_Test(uint32_t param);




void Task_PWM_Manage(uint32_t param);


u8 Get_TaskId_From_TestItem(u8 test_id);








bool Program_Words_MCU_Flash(uint32_t baseAddr, uint32_t offset, uint8_t* data);
bool Erase_MCU_Flash(uint32_t startSec, uint32_t NbOfSec);
uint32_t GetSector(uint32_t Address);


void Console_port_init();
void Console_Rx_Cb();

void Init_test_item_list();


void Buzzor_Start(u16 tick);
void Buzzor_Operate();

#endif /* __MESSAGE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
