

/* Includes ------------------------------------------------------------------*/


//#include "tusb.h"

#include "main.h"
#include "taskDef.h"
#include "myTasks.h"
#include "myQueue.h"
#include "kernel.h"

u32 tick_buzzor = 0;


extern test_item_t* test_item_active;


void Task_Idle(uint32_t param){

  while(1){


  }


}


void Task_Timer(uint32_t param){

  while(1){

  }
}










u8 Get_TaskId_From_TestItem(u8 test_id){

    switch(test_item_active->test_id){

        case eTest_cover_open:     return eID_PWM_GEN_TEST;

        case eTest_valve:   return eID_GPIO_OUT_TEST;

        case eTest_pressure:    return eID_MCU_ADC_TEST;

        case eTest_limit_sens:  return eID_GPIO_INPUT_TEST;

        case eTest_temperature:  return eID_TEMPERATURE_TEST;    

        default:    return eId_UNVALID_TASK;

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



bool Program_Words_MCU_Flash(uint32_t baseAddr, uint32_t offset, uint8_t* data){

	uint32_t Address = baseAddr;

	if(baseAddr % 4 != 0 || offset % 4 != 0){
		
		PRINT_DBG("Offset Error!! offset must 4*N value\r\n");
		return false;
	}


	HAL_FLASH_Unlock();


	for (uint32_t i=0; i<offset; i+=4)
    {

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, *((uint32_t*)(data+i)) ) == HAL_OK)
        { 
          Address = Address + 4;
        }
        else {
          PRINT_DBG("Flash programming error!!\r\n");
		  return false;
        }
    }

	HAL_FLASH_Lock();

	return true;

}


bool Erase_MCU_Flash(uint32_t startSec, uint32_t NbOfSec){

	uint32_t FirstSector, SECTORError = 0;
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
		return false;
	}

	HAL_FLASH_Lock();

	return true;
}




uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;

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


