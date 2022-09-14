

/* Includes ------------------------------------------------------------------*/

#include "task_basic.h"


u32 tick_buzzor = 0;



test_item_t* test_item_active;

test_item_t test_item[eTEST_MAX] = {
    {"PWM DMA\0",       eTEST_PWM_DMA,      NULL,   NULL},
    {"Close Sensor\0",  eTEST_CLOSE_SENS,   NULL,   NULL},
    {"Temperature\0",   eTEST_TEMPERATURE,  NULL,   NULL},
    {"Remote Comm.\0",  eTEST_REMOTE_COMM,  NULL,   NULL},
    {"EEPROM R/W\0",    eTEST_EEPROM_RW,    NULL,   NULL},
    {"GPIO Out\0",      eTEST_GPIO_CTL,     NULL,   NULL},
    {"ADC Check\0",     eTEST_ADC_CHECK,     NULL,   NULL},
};


void Task_Idle(u32 param){

  while(1){


  }


}


void Task_Timer(u32 param){

  while(1){

  }
}







void Init_test_item_list(){

    test_item[0].next_item = &test_item[1];
    test_item[0].prev_item = &test_item[eTEST_MAX-1];

    for(u8 idx=1; idx<eTEST_MAX-1; ++idx){
        test_item[idx].next_item = &test_item[idx+1];
        test_item[idx].prev_item = &test_item[idx-1];
    }

    test_item[eTEST_MAX-1].next_item = &test_item[0];
    test_item[eTEST_MAX-1].prev_item = &test_item[eTEST_MAX-2];

    test_item_active = &test_item[4];

}


u8 Get_TaskId_From_TestItem(u8 test_id){

    switch(test_item_active->test_id){

        case eTEST_PWM_DMA:       return eTID_PWM_GEN_TEST;
        case eTEST_CLOSE_SENS:    return eTID_GPIO_INPUT_TEST;
        case eTEST_TEMPERATURE:   return eTID_TEMPERATURE_TEST; 

        case eTEST_REMOTE_COMM:   return eTID_TIMER;
        case eTEST_EEPROM_RW:     return eTID_TIMER;

        case eTEST_GPIO_CTL:      return eTID_GPIO_OUT_TEST;
        case eTEST_ADC_CHECK:     return eTID_MCU_ADC_TEST;        
        default:                  return eId_UNVALID_TASK;

    }

}




void Buzzor_Start(u16 tick){

  tick_buzzor = tick;

}

void Buzzor_Operate(){

  if(tick_buzzor > 0){
    BUZZOR_CTL(GPIO_PIN_SET);
    tick_buzzor--;
  }
  else {
    BUZZOR_CTL(GPIO_PIN_RESET);
  }
}



s8 Program_Words_MCU_Flash(u32 baseAddr, u32 offset, u8* data){

	u32 Address = baseAddr;

	if(baseAddr % 4 != 0 || offset % 4 != 0){
		
		PRINT_DBG("Offset Error!! offset must 4*N value\r\n");
		return 1;
	}


	HAL_FLASH_Unlock();


	for (u32 i=0; i<offset; i+=4)
    {

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, *((u32*)(data+i)) ) == HAL_OK)
        { 
          Address = Address + 4;
        }
        else {
          PRINT_DBG("Flash programming error!!\r\n");
		      return 2;
        }
    }

	HAL_FLASH_Lock();

	return 0;

}


s8 Erase_MCU_Flash(u32 startSec, u32 NbOfSec){

	u32 FirstSector, SECTORError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;
	
	FirstSector = GetSector(startSec);
	EraseInitStruct.TypeErase	 = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector		 = FirstSector;
	EraseInitStruct.NbSectors	 = NbOfSec;

	PRINT_DBG("Get sector info FirstSector:%ld,  NbOfSec:%ld\n\r", FirstSector, EraseInitStruct.NbSectors);


	HAL_FLASH_Unlock();
	// erase start
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		PRINT_DBG("Flash erasing error!!\r\n");
		/* Infinite loop */
		return 1;
	}

	HAL_FLASH_Lock();

	return 0;
}




u32 GetSector(u32 Address)
{
  u32 sector = 0;

  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;
  }
  else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_7) */
  {
    sector = FLASH_SECTOR_7;
  }
  return sector;
}


