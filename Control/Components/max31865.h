#ifndef __MAX31865_H__
#define __MAX31865_H__

#include "main.h"
#include "kernel.h"

#define TEMP_SPI    hspi4

#define SPI_SELECT      HAL_GPIO_WritePin(GPIOE, SPI4_NSS_TEMP_Pin, GPIO_PIN_RESET);
#define SPI_DESELECT    HAL_GPIO_WritePin(GPIOE, SPI4_NSS_TEMP_Pin, GPIO_PIN_SET);

#define MAX31865_RREF      430.0f
#define MAX31865_RNOMINAL  100.0f



#define MAX31856_CONFIG_REG             0x00
#define MAX31856_CONFIG_BIAS            0x80
#define MAX31856_CONFIG_MODEAUTO        0x40
#define MAX31856_CONFIG_MODEOFF         0x00
#define MAX31856_CONFIG_1SHOT           0x20
#define MAX31856_CONFIG_3WIRE           0x10
#define MAX31856_CONFIG_24WIRE          0x00
#define MAX31856_CONFIG_FAULTSTAT       0x02
#define MAX31856_CONFIG_FILT50HZ        0x01
#define MAX31856_CONFIG_FILT60HZ        0x00


#define MAX31856_RTDMSB_REG             0x01
#define MAX31856_RTDLSB_REG             0x02
#define MAX31856_HFAULTMSB_REG          0x03
#define MAX31856_HFAULTLSB_REG          0x04
#define MAX31856_LFAULTMSB_REG          0x05
#define MAX31856_LFAULTLSB_REG          0x06
#define MAX31856_FAULTSTAT_REG          0x07


#define MAX31865_FAULT_HIGHTHRESH       0x80
#define MAX31865_FAULT_LOWTHRESH        0x40
#define MAX31865_FAULT_REFINLOW         0x20
#define MAX31865_FAULT_REFINHIGH        0x10
#define MAX31865_FAULT_RTDINLOW         0x08
#define MAX31865_FAULT_OVUV             0x04

#define MAX31865_CONFIG_DISABLE         0
#define MAX31865_CONFIG_ENABLE          1
#define MAX31865_CONFIG_3WIRE           3
#define MAX31865_CONFIG_50HZ            50
#define MAX31865_CONFIG_60HZ            60
#define MAX31865_CONFIG_LOW_THRESHOLD   0x3B88  // (-0.016)
#define MAX31865_CONFIG_HIGH_THRESHOLD  0x68B4  // (200.xx)

#define RTD_A 3.9083e-3
#define RTD_B -5.775e-7


extern u8 rtd_fault_sts;

void Max31865_readRegisterN(uint8_t addr, uint8_t *buffer, uint8_t n);
uint8_t Max31865_readRegister8(uint8_t addr);
uint16_t Max31865_readRegister16(uint8_t addr);
void Max31865_writeRegister8(uint8_t addr, uint8_t data);


uint8_t Max31865_readFault(void);
void Max31865_clearFault(void);



void Max31865_enableBias(uint8_t enable);
void Max31865_autoConvert(uint8_t enable);
void Max31865_setWires(uint8_t numWires);


void Max31865_setFilter(uint8_t filterHz);
void Max31865_setThresholds(uint16_t lower, uint16_t upper);


uint16_t Max31865_readRTD (void);
void  Max31865_init(uint8_t numwires, uint8_t filterHz);
bool Max31865_readTempC(float *readTemp);


#endif