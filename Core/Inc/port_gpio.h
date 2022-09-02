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

//HAL_StatusTypeDef HAL_TIM_PWM_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
//HAL_StatusTypeDef HAL_TIM_PWM_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);




#define PWM_START_DMA(TIMER, CH, DATA_ARR, CNT)	(TIMER).State = HAL_TIM_STATE_READY;						\
												HAL_TIM_PWM_Start_DMA(&(TIMER), (CH), (DATA_ARR), (CNT))
											

#define PWM_STOP_DMA(TIMER, CH)					HAL_TIM_PWM_Stop_DMA( &(TIMER), (CH) )


// Cover Opener
#define GPIOx_COVER_DR_L		GPIO_L_DC_PWM_GPIO_Port
#define GPIO_PIN_COVER_DR_L		GPIO_L_DC_PWM_Pin
#define GPIOx_COVER_DR_R		GPIO_R_DC_PWM_GPIO_Port
#define GPIO_PIN_COVER_DR_R		GPIO_R_DC_PWM_Pin
#define GPIOx_COVER_EN			GPIO_R_DC_PWM_GPIO_Port
#define GPIO_PIN_COVER_EN		GPIO_R_DC_PWM_Pin

#define PWM_TIMER_COVER			htim3
#define PWM_CHANNEL_COVER		TIM_CHANNEL_3
#define PWM_SOURCE_COVER		(TIM_TypeDef*)(TIM3_BASE)



//  Cover Side Locker Left
#define GPIOx_SIDE_L_DR			GPIO_STEP_L_DIR_GPIO_Port
#define GPIO_PIN_SIDE_L_DR		GPIO_STEP_L_DIR_Pin
#define GPIOx_SIDE_L_EN			GPIO_STEP_L_EN_GPIO_Port
#define GPIO_PIN_SIDE_L_EN		GPIO_STEP_L_EN_Pin

#define PWM_TIMER_SIDE_L		htim1
#define PWM_CHANNEL_SIDE_L		TIM_CHANNEL_1
#define PWM_SOURCE_SIDE_L		(TIM_TypeDef*)(TIM1_BASE)



// Cover Side Locker Right
#define GPIOx_SIDE_R_DR			GPIO_STEP_R_DIR_GPIO_Port
#define GPIO_PIN_SIDE_R_DR		GPIO_STEP_R_DIR_Pin
#define GPIOx_SIDE_R_EN			GPIO_STEP_R_EN_GPIO_Port
#define GPIO_PIN_SIDE_R_EN		GPIO_STEP_R_EN_Pin

#define PWM_TIMER_SIDE_R			htim2
#define PWM_CHANNEL_SIDE_R		TIM_CHANNEL_2
#define PWM_SOURCE_SIDE_R		(TIM_TypeDef*)(TIM2_BASE)






// Close Sensors
#define READ_COVER_RIGHT_LOCK		HAL_GPIO_ReadPin(STEP_R1_SENS_GPIO_Port, 	STEP_R1_SENS_Pin)
#define READ_COVER_RIGHT_OPEN		HAL_GPIO_ReadPin(STEP_R2_SENS_GPIO_Port, 	STEP_R2_SENS_Pin)
#define READ_COVER_LEFT_LOCK		HAL_GPIO_ReadPin(STEP_L1_SENS_GPIO_Port, 	STEP_L1_SENS_Pin)
#define READ_COVER_LEFT_OPEN		HAL_GPIO_ReadPin(STEP_L2_SENS_GPIO_Port, 	STEP_L2_SENS_Pin)
#define READ_COVER_TILT_LOCK		HAL_GPIO_ReadPin(ACTUATOR_UP_SENS_GPIO_Port, ACTUATOR_UP_SENS_Pin)
#define READ_COVER_TILT_OPEN	    HAL_GPIO_ReadPin(ACTUATOR_DN_SENS_GPIO_Port, ACTUATOR_DN_SENS_Pin)



// Button State(On/Off) Read
#define READ_ENTER_BTN			HAL_GPIO_ReadPin(INT_ENTER_BTN_GPIO_Port, 	INT_ENTER_BTN_Pin)
#define READ_MODE_BTN			HAL_GPIO_ReadPin(INT_MODE_BTN_GPIO_Port, 	INT_MODE_BTN_Pin)
#define READ_CLEAN_BTN			HAL_GPIO_ReadPin(INT_CLEAN_BTN_GPIO_Port, 	INT_CLEAN_BTN_Pin)
#define READ_COOK_BTN			HAL_GPIO_ReadPin(INT_COOK_BTN_GPIO_Port, 	INT_COOK_BTN_Pin)

#define READ_UP_BTN				HAL_GPIO_ReadPin(INT_UP_BTN_GPIO_Port, 		INT_UP_BTN_Pin)
#define READ_DOWN_BTN			HAL_GPIO_ReadPin(INT_DOWN_BTN_GPIO_Port, 	INT_DOWN_BTN_Pin)
#define READ_LEFT_BTN			HAL_GPIO_ReadPin(INT_LEFT_BTN_GPIO_Port, 	INT_LEFT_BTN_Pin)
#define READ_RIGHT_BTN			HAL_GPIO_ReadPin(INT_RIGHT_BTN_GPIO_Port, 	INT_RIGHT_BTN_Pin)

#define BUTTON_PRESSED			GPIO_PIN_RESET
#define BUTTON_RELEASED			GPIO_PIN_SET


// Button Code
#define BTN_CODE_ENTER	(uint16_t)(0x01 << 15)
#define BTN_CODE_MODE	(uint16_t)(0x01 << 5)
#define BTN_CODE_CLEAN	(uint16_t)(0x01 << 6)
#define BTN_CODE_COOK	(uint16_t)(0x01 << 8)

#define BTN_CODE_LEFT	(uint16_t)(0x01 << 9)
#define BTN_CODE_RIGHT	(uint16_t)(0x01 << 12)
#define BTN_CODE_UP		(uint16_t)(0x01 << 13)
#define BTN_CODE_DOWN	(uint16_t)(0x01 << 14)



// Button LED
#define LED_COOK_AUTO_ONOFF(X)		HAL_GPIO_WritePin(COOK_AUTO_LED_GPIO_Port, COOK_AUTO_LED_Pin, (X))
#define LED_COOK_MANUAL_ONOFF(X)	HAL_GPIO_WritePin(COOK_MAN_LED_GPIO_Port, COOK_MAN_LED_Pin, (X))
#define LED_CLEAN_ONOFF(X)		HAL_GPIO_WritePin(CLEAN_LED_GPIO_Port, CLEAN_LED_Pin, (X))
#define LED_COOK_ONOFF(X)		HAL_GPIO_WritePin(COOK_LED_GPIO_Port, COOK_LED_Pin, (X))
#define LED_ENTER_ONOFF(X)		HAL_GPIO_WritePin(ENTER_BTN_LED_GPIO_Port, ENTER_BTN_LED_Pin, (X))



// Ball&Sol valve control
#define BALL_VALVE_01_OPEN(X)	HAL_GPIO_WritePin(GPIO_BALL_VALVE_01_OPEN_GPIO_Port, GPIO_BALL_VALVE_01_OPEN_Pin, (X))
#define BALL_VALVE_02_OPEN(X)	HAL_GPIO_WritePin(GPIO_BALL_VALVE_02_OPEN_GPIO_Port, GPIO_BALL_VALVE_02_OPEN_Pin, (X))

#define BALL_VALVE_01_CLOSE(X)	HAL_GPIO_WritePin(GPIO_BALL_VALVE_01_CLOSE_GPIO_Port, GPIO_BALL_VALVE_01_CLOSE_Pin, (X))
#define BALL_VALVE_02_CLOSE(X)	HAL_GPIO_WritePin(GPIO_BALL_VALVE_02_CLOSE_GPIO_Port, GPIO_BALL_VALVE_02_CLOSE_Pin, (X))

#define SOL_VALVE_CTL(X)	HAL_GPIO_WritePin(GPIO_SOL_VALVE_GPIO_Port, GPIO_SOL_VALVE_Pin, (X))


//pump ac motor control
#define PUMP_AC_MOTOR_R_CTL(X)	HAL_GPIO_WritePin(GPIO_AC_MOTOR_R_GPIO_Port, GPIO_AC_MOTOR_R_Pin, (X))
#define PUMP_AC_MOTOR_F_CTL(X)	HAL_GPIO_WritePin(GPIO_AC_MOTOR_F_GPIO_Port, GPIO_AC_MOTOR_F_Pin, (X))

// Fan Control
#define FAN_24V_01_CTL(X)		HAL_GPIO_WritePin(GPIO_FAN24V_01_GPIO_Port, GPIO_FAN24V_01_Pin, (X))
#define FAN_24V_02_CTL(X)		HAL_GPIO_WritePin(GPIO_FAN24V_02_GPIO_Port, GPIO_FAN24V_02_Pin, (X))
#define FAN_24V_03_CTL(X)		HAL_GPIO_WritePin(GPIO_FAN24V_03_GPIO_Port, GPIO_FAN24V_03_Pin, (X))


// Buzzor
#define BUZZOR_CTL(X)		HAL_GPIO_WritePin(GPIO_BUZZOR_GPIO_Port, GPIO_BUZZOR_Pin, (X))




#define PWM_START(TIMER, CH)		HAL_TIM_PWM_Start(&(TIMER), (CH))
#define PWM_STOP(TIMER, CH)			HAL_TIM_PWM_Stop(&(TIMER), (CH))

#define SIDE_LOCKER_SPD_SET(X)		PWM_TIMER_SIDE_L.Instance->PSC = (X/2) - 1;	\
									PWM_TIMER_SIDE_R.Instance->PSC = (X) - 1




#endif

