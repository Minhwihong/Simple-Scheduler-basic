#ifndef __PORT_GPIO_H__
#define __PORT_GPIO_H__

#include "stm32f4xx_hal.h"
#include "ModelConfig.h"
#include "main.h"



#define UART_DBG	huart6
#define UART_MES	huart6
#define UART_IH		huart1
#define UART_MP3	huart3


#define I2C_EEPROM	hi2c2



#define PWM_START_DMA(TIMER, CH, DATA_ARR, CNT)	(TIMER).State = HAL_TIM_STATE_READY;						\
												HAL_TIM_PWM_Start_DMA(&(TIMER), (CH), (DATA_ARR), (CNT))
											

#define PWM_STOP_DMA(TIMER, CH)					HAL_TIM_PWM_Stop_DMA( &(TIMER), (CH) )


// Cover Opener
#define GPIOx_MOTOR_DR_L		(GPIO_L_DC_PWM_GPIO_Port)
#define GPIO_PIN_MOTOR_DR_L		(GPIO_L_DC_PWM_Pin)
#define GPIOx_MOTOR_DR_R		(GPIO_R_DC_PWM_GPIO_Port)
#define GPIO_PIN_MOTOR_DR_R		(GPIO_R_DC_PWM_Pin)


#define PWM_TIMER				(htim3)
#define PWM_CHANNEL				(TIM_CHANNEL_3)
#define PWM_SOURCE				(TIM_TypeDef*)(TIM3_BASE)




// GPIO Input
#define READ_SENSOR_1			HAL_GPIO_ReadPin(STEP_R1_SENS_GPIO_Port, 	STEP_R1_SENS_Pin)
#define READ_SENSOR_2			HAL_GPIO_ReadPin(STEP_R2_SENS_GPIO_Port, 	STEP_R2_SENS_Pin)
#define READ_SENSOR_3			HAL_GPIO_ReadPin(STEP_L1_SENS_GPIO_Port, 	STEP_L1_SENS_Pin)




// Button Code
#define BTN_CODE_ENTER	(uint16_t)(0x01 << 15)
#define BTN_CODE_MODE	(uint16_t)(0x01 << 5)
#define BTN_CODE_FUNC1	(uint16_t)(0x01 << 6)
#define BTN_CODE_FUNC2	(uint16_t)(0x01 << 8)

#define BTN_CODE_LEFT	(uint16_t)(0x01 << 9)
#define BTN_CODE_RIGHT	(uint16_t)(0x01 << 12)
#define BTN_CODE_UP		(uint16_t)(0x01 << 13)
#define BTN_CODE_DOWN	(uint16_t)(0x01 << 14)




// 
#define GPIO_OUTPUT_CTL01(X)	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, (X))
#define GPIO_OUTPUT_CTL02(X)	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, (X))



// Buzzor
#define BUZZOR_CTL(X)		HAL_GPIO_WritePin(GPIO_BUZZOR_GPIO_Port, GPIO_BUZZOR_Pin, (X))




#define PWM_START(TIMER, CH)		HAL_TIM_PWM_Start(&(TIMER), (CH))
#define PWM_STOP(TIMER, CH)			HAL_TIM_PWM_Stop(&(TIMER), (CH))

#define SIDE_LOCKER_SPD_SET(X)		PWM_TIMER_SIDE_L.Instance->PSC = (X/2) - 1;	\
									PWM_TIMER_SIDE_R.Instance->PSC = (X) - 1




#endif

