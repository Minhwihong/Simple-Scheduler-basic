#ifndef __TASK_BASIC_H__
#define __TASK_BASIC_H__

#include "main.h"
#include "ModelConfig.h"
#include "taskDef.h"
#include "myQueue.h"
#include "kernel.h"
#include "manage_task.h"



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


u8 Get_TaskId_From_TestItem(u8 test_id);



s8 Program_Words_MCU_Flash(u32 baseAddr, u32 offset, u8* data);
s8 Erase_MCU_Flash(u32 startSec, u32 NbOfSec);
u32 GetSector(u32 Address);


void Console_port_init();
void Console_Rx_Cb();

void Init_test_item_list();


void Buzzor_Start(u16 tick);
void Buzzor_Operate();


#endif