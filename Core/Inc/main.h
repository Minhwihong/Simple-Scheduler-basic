/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "printf.h"
//#include <stdio.h>
#include "ModelConfig.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim8;

extern CAN_HandleTypeDef hcan1;

extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;

extern SPI_HandleTypeDef hspi3;
extern SPI_HandleTypeDef hspi4;

extern myInfo_t	myInfo;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 16 Kbytes */	// Bootloader
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base address of Sector 1, 16 Kbytes */	// Recipe Information
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base address of Sector 2, 16 Kbytes */	// Calibration Data
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 Kbytes */	// MES Sign
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base address of Sector 4, 64 Kbytes */	// MES & Testing firmware
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base address of Sector 5, 128 Kbytes */	// Application Sector
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base address of Sector 6, 128 Kbytes */	
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base address of Sector 7, 128 Kbytes */	// Buffer Sector

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void MX_SPI4_Init(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCU_LED0_Pin GPIO_PIN_2
#define MCU_LED0_GPIO_Port GPIOE
#define GPIO_FAN24V_01_Pin GPIO_PIN_3
#define GPIO_FAN24V_01_GPIO_Port GPIOE
#define GPIO_FAN24V_02_Pin GPIO_PIN_4
#define GPIO_FAN24V_02_GPIO_Port GPIOE
#define INT_AM_BTN_Pin GPIO_PIN_5
#define INT_AM_BTN_GPIO_Port GPIOE
#define INT_AM_BTN_EXTI_IRQn EXTI9_5_IRQn
#define INT_CLEAN_BTN_Pin GPIO_PIN_6
#define INT_CLEAN_BTN_GPIO_Port GPIOE
#define INT_CLEAN_BTN_EXTI_IRQn EXTI9_5_IRQn
#define INT_UP_BTN_Pin GPIO_PIN_13
#define INT_UP_BTN_GPIO_Port GPIOC
#define INT_UP_BTN_EXTI_IRQn EXTI15_10_IRQn
#define INT_DOWN_BTN_Pin GPIO_PIN_14
#define INT_DOWN_BTN_GPIO_Port GPIOC
#define INT_DOWN_BTN_EXTI_IRQn EXTI15_10_IRQn
#define INT_ENTER_BTN_Pin GPIO_PIN_15
#define INT_ENTER_BTN_GPIO_Port GPIOC
#define INT_ENTER_BTN_EXTI_IRQn EXTI15_10_IRQn
#define ACTUATOR_UP_SENS_Pin GPIO_PIN_0
#define ACTUATOR_UP_SENS_GPIO_Port GPIOC
#define ACTUATOR_DN_SENS_Pin GPIO_PIN_1
#define ACTUATOR_DN_SENS_GPIO_Port GPIOC
#define STEP_L1_SENS_Pin GPIO_PIN_2
#define STEP_L1_SENS_GPIO_Port GPIOC
#define STEP_L2_SENS_Pin GPIO_PIN_3
#define STEP_L2_SENS_GPIO_Port GPIOC
#define ADC_PRESS_SIG_Pin GPIO_PIN_0
#define ADC_PRESS_SIG_GPIO_Port GPIOA
#define PWM5_CH2_TEST1_Pin GPIO_PIN_1
#define PWM5_CH2_TEST1_GPIO_Port GPIOA
#define GPIO_TEST1_DIR_Pin GPIO_PIN_2
#define GPIO_TEST1_DIR_GPIO_Port GPIOA
#define ADC_DC_L_IS_Pin GPIO_PIN_3
#define ADC_DC_L_IS_GPIO_Port GPIOA
#define STEP_R1_SENS_Pin GPIO_PIN_4
#define STEP_R1_SENS_GPIO_Port GPIOA
#define STEP_R2_SENS_Pin GPIO_PIN_5
#define STEP_R2_SENS_GPIO_Port GPIOA
#define GPIO_L_DC_PWM_Pin GPIO_PIN_6
#define GPIO_L_DC_PWM_GPIO_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_7
#define OLED_DC_GPIO_Port GPIOA
#define OLED_RES_Pin GPIO_PIN_4
#define OLED_RES_GPIO_Port GPIOC
#define UART3_RX_MP3_Pin GPIO_PIN_5
#define UART3_RX_MP3_GPIO_Port GPIOC
#define PWM3_CH3_DC_EN_Pin GPIO_PIN_0
#define PWM3_CH3_DC_EN_GPIO_Port GPIOB
#define ADC_DC_R_IS_Pin GPIO_PIN_1
#define ADC_DC_R_IS_GPIO_Port GPIOB
#define GPIO_BALL_VALVE_02_OPEN_Pin GPIO_PIN_2
#define GPIO_BALL_VALVE_02_OPEN_GPIO_Port GPIOB
#define GPIO_FAN24V_03_Pin GPIO_PIN_7
#define GPIO_FAN24V_03_GPIO_Port GPIOE
#define GPIO_R_DC_PWM_Pin GPIO_PIN_8
#define GPIO_R_DC_PWM_GPIO_Port GPIOE
#define PWM1_CH1_STEP_L_Pin GPIO_PIN_9
#define PWM1_CH1_STEP_L_GPIO_Port GPIOE
#define GPIO_STEP_L_DIR_Pin GPIO_PIN_10
#define GPIO_STEP_L_DIR_GPIO_Port GPIOE
#define SPI4_NSS_TEMP_Pin GPIO_PIN_11
#define SPI4_NSS_TEMP_GPIO_Port GPIOE
#define SPI4_SCK_TEMP_Pin GPIO_PIN_12
#define SPI4_SCK_TEMP_GPIO_Port GPIOE
#define SPI4_MISO_TEMP_Pin GPIO_PIN_13
#define SPI4_MISO_TEMP_GPIO_Port GPIOE
#define GPIO_STEP_L_EN_Pin GPIO_PIN_14
#define GPIO_STEP_L_EN_GPIO_Port GPIOE
#define MCU_LED1_Pin GPIO_PIN_15
#define MCU_LED1_GPIO_Port GPIOE
#define I2C2_SCL_EEPROM_Pin GPIO_PIN_10
#define I2C2_SCL_EEPROM_GPIO_Port GPIOB
#define INT_RIGHT_BTN_Pin GPIO_PIN_12
#define INT_RIGHT_BTN_GPIO_Port GPIOB
#define INT_RIGHT_BTN_EXTI_IRQn EXTI15_10_IRQn
#define GPIO_AM_LED01_Pin GPIO_PIN_13
#define GPIO_AM_LED01_GPIO_Port GPIOB
#define GPIO_AM_LED02_Pin GPIO_PIN_14
#define GPIO_AM_LED02_GPIO_Port GPIOB
#define GPIO_MP3_BUSY_Pin GPIO_PIN_15
#define GPIO_MP3_BUSY_GPIO_Port GPIOB
#define UART3_TX_MP3_Pin GPIO_PIN_8
#define UART3_TX_MP3_GPIO_Port GPIOD
#define GPIO_CLEAN_LED_Pin GPIO_PIN_9
#define GPIO_CLEAN_LED_GPIO_Port GPIOD
#define GPIO_TEST2_DIR_Pin GPIO_PIN_10
#define GPIO_TEST2_DIR_GPIO_Port GPIOD
#define GPIO_TEST2_EN_Pin GPIO_PIN_11
#define GPIO_TEST2_EN_GPIO_Port GPIOD
#define PWM4_CH1_TEST2_Pin GPIO_PIN_12
#define PWM4_CH1_TEST2_GPIO_Port GPIOD
#define GPIO_COOK_LED_Pin GPIO_PIN_13
#define GPIO_COOK_LED_GPIO_Port GPIOD
#define GPIO_ENTER_LED_Pin GPIO_PIN_14
#define GPIO_ENTER_LED_GPIO_Port GPIOD
#define TEST1_Pin GPIO_PIN_15
#define TEST1_GPIO_Port GPIOD
#define UART6_TX_DBG_Pin GPIO_PIN_6
#define UART6_TX_DBG_GPIO_Port GPIOC
#define UART6_RX_DBG_Pin GPIO_PIN_7
#define UART6_RX_DBG_GPIO_Port GPIOC
#define PWM8_CH3_TEST_Pin GPIO_PIN_8
#define PWM8_CH3_TEST_GPIO_Port GPIOC
#define I2C3_SDA_RES_Pin GPIO_PIN_9
#define I2C3_SDA_RES_GPIO_Port GPIOC
#define I2C3_SCL_RES_Pin GPIO_PIN_8
#define I2C3_SCL_RES_GPIO_Port GPIOA
#define UART1_TX_IH_Pin GPIO_PIN_9
#define UART1_TX_IH_GPIO_Port GPIOA
#define UART1_RX_IH_Pin GPIO_PIN_10
#define UART1_RX_IH_GPIO_Port GPIOA
#define TEST2_Pin GPIO_PIN_11
#define TEST2_GPIO_Port GPIOA
#define TEST3_Pin GPIO_PIN_12
#define TEST3_GPIO_Port GPIOA
#define SPI3_NSS_Pin GPIO_PIN_15
#define SPI3_NSS_GPIO_Port GPIOA
#define GPIO_TEST1_EN_Pin GPIO_PIN_11
#define GPIO_TEST1_EN_GPIO_Port GPIOC
#define I2C2_SDA_EEPROM_Pin GPIO_PIN_12
#define I2C2_SDA_EEPROM_GPIO_Port GPIOC
#define GPIO_AC_MOTOR_R_Pin GPIO_PIN_2
#define GPIO_AC_MOTOR_R_GPIO_Port GPIOD
#define GPIO_AC_MOTOR_F_Pin GPIO_PIN_3
#define GPIO_AC_MOTOR_F_GPIO_Port GPIOD
#define GPIO_SOL_VALVE_Pin GPIO_PIN_4
#define GPIO_SOL_VALVE_GPIO_Port GPIOD
#define GPIO_BALL_VALVE_01_CLOSE_Pin GPIO_PIN_5
#define GPIO_BALL_VALVE_01_CLOSE_GPIO_Port GPIOD
#define GPIO_STEP_R_EN_Pin GPIO_PIN_6
#define GPIO_STEP_R_EN_GPIO_Port GPIOD
#define GPIO_STEP_R_DIR_Pin GPIO_PIN_7
#define GPIO_STEP_R_DIR_GPIO_Port GPIOD
#define PWM2_CH2_STEP_R_Pin GPIO_PIN_3
#define PWM2_CH2_STEP_R_GPIO_Port GPIOB
#define GPIO_BALL_VALVE_02_CLOSE_Pin GPIO_PIN_7
#define GPIO_BALL_VALVE_02_CLOSE_GPIO_Port GPIOB
#define INT_COOK_BTN_Pin GPIO_PIN_8
#define INT_COOK_BTN_GPIO_Port GPIOB
#define INT_COOK_BTN_EXTI_IRQn EXTI9_5_IRQn
#define INT_LEFT_BTN_Pin GPIO_PIN_9
#define INT_LEFT_BTN_GPIO_Port GPIOB
#define INT_LEFT_BTN_EXTI_IRQn EXTI9_5_IRQn
#define GPIO_BUZZOR_Pin GPIO_PIN_0
#define GPIO_BUZZOR_GPIO_Port GPIOE
#define GPIO_BALL_VALVE_01_OPEN_Pin GPIO_PIN_1
#define GPIO_BALL_VALVE_01_OPEN_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
