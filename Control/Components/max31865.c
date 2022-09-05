#include "max31865.h"
#include "myTasks.h"



//static uint16_t rtd_raw;
//static uint16_t rtd_remove_fault;
//static uint8_t fault;

//static bool is_ready_to_read_rtd;

//static bool is_pt100_ok;
//static float temp;


u8 rtd_fault_sts = 0;


void Max31865_readRegisterN(uint8_t addr, uint8_t *buffer, uint8_t n){
    uint8_t tmp = 0xFF;
	addr &= 0x7F;
    
	SPI_SELECT;
    HAL_SPI_Transmit(&TEMP_SPI, &addr, 1, HAL_MAX_DELAY);
    
	while (n--){
        HAL_SPI_TransmitReceive(&TEMP_SPI, &tmp, buffer, 1, HAL_MAX_DELAY);
		buffer++;
	}
    
	SPI_DESELECT;  
	Kernel_Delay_Set(eTID_TEMPERATURE_TEST, 2);  
}

uint8_t Max31865_readRegister8(uint8_t addr){
    uint8_t ret = 0;
    
	Max31865_readRegisterN(addr, &ret, 1);
	return ret;  
}

uint16_t Max31865_readRegister16(uint8_t addr){
	uint8_t buffer[2] = {0, 0};
    uint16_t ret;
    
	Max31865_readRegisterN(addr, buffer, 2);
    
    ret = ((buffer[0] << 8) | (buffer[1]));
    
	return ret;
}

void Max31865_writeRegister8(uint8_t addr, uint8_t data){     
    addr |= 0x80;
    
	SPI_SELECT;
    
	HAL_SPI_Transmit(&TEMP_SPI, &addr, 1, HAL_MAX_DELAY);   
	HAL_SPI_Transmit(&TEMP_SPI, &data, 1, HAL_MAX_DELAY);
    
    SPI_DESELECT;
	Kernel_Delay_Set(eTID_TEMPERATURE_TEST, 2);  
}

uint8_t Max31865_readFault(void){

	rtd_fault_sts = Max31865_readRegister8(MAX31856_FAULTSTAT_REG);

    return rtd_fault_sts;
}

void Max31865_clearFault(void){
	uint8_t buffer = Max31865_readRegister8(MAX31856_CONFIG_REG);
    
	buffer &= ~0x2C;
	buffer |= MAX31856_CONFIG_FAULTSTAT;
    
	Max31865_writeRegister8(MAX31856_CONFIG_REG, buffer);
}

void Max31865_enableBias(uint8_t enable){
    uint8_t buffer = Max31865_readRegister8(MAX31856_CONFIG_REG);

	if(enable){
		buffer |= MAX31856_CONFIG_BIAS;
    }
	else{
		buffer &= ~MAX31856_CONFIG_BIAS;
    }
    
	Max31865_writeRegister8(MAX31856_CONFIG_REG, buffer);
}

void Max31865_autoConvert(uint8_t enable){
	uint8_t buffer = Max31865_readRegister8(MAX31856_CONFIG_REG);
    
	if(enable){
		buffer |= MAX31856_CONFIG_MODEAUTO;
    }
	else{
		buffer &= ~MAX31856_CONFIG_MODEAUTO; 
    }
    
	Max31865_writeRegister8(MAX31856_CONFIG_REG, buffer);
}

void Max31865_setWires(uint8_t numWires){
	uint8_t buffer = Max31865_readRegister8(MAX31856_CONFIG_REG);
    
	if(numWires == MAX31856_CONFIG_3WIRE){
		buffer |= MAX31856_CONFIG_3WIRE;
    }
	else{
		buffer &= ~MAX31856_CONFIG_3WIRE;
    }
    
	Max31865_writeRegister8(MAX31856_CONFIG_REG, buffer);
}

void Max31865_setFilter(uint8_t filterHz){
	uint8_t buffer = Max31865_readRegister8(MAX31856_CONFIG_REG);
    
	if(filterHz == MAX31865_CONFIG_50HZ){
		buffer |= MAX31856_CONFIG_FILT50HZ;
    }
	else{
		buffer &= ~MAX31856_CONFIG_FILT50HZ;
    }
    
	Max31865_writeRegister8(MAX31856_CONFIG_REG, buffer);
}

void Max31865_setThresholds(uint16_t lower, uint16_t upper) {
    Max31865_writeRegister8(MAX31856_LFAULTLSB_REG, lower & 0xFF);
    Max31865_writeRegister8(MAX31856_LFAULTMSB_REG, lower >> 8);
    Max31865_writeRegister8(MAX31856_HFAULTLSB_REG, upper & 0xFF);
    Max31865_writeRegister8(MAX31856_HFAULTMSB_REG, upper >> 8);
}





uint16_t Max31865_readRTD (void){
    uint8_t buffer;
    uint16_t rtd;
    
	
	Max31865_clearFault();
	Max31865_enableBias(MAX31865_CONFIG_ENABLE);
	
    buffer = Max31865_readRegister8(MAX31856_CONFIG_REG);
    
	buffer |= MAX31856_CONFIG_1SHOT;
	Max31865_writeRegister8(MAX31856_CONFIG_REG, buffer);
    
    //while(is_ready_to_read_rtd != true){
        
    //}

	//HAL_Delay(70);
	Kernel_Delay_Set(eTID_TEMPERATURE_TEST, 50);
    
	rtd = Max31865_readRegister16(MAX31856_RTDMSB_REG);
    
	Max31865_enableBias(MAX31865_CONFIG_DISABLE);
	
    //is_ready_to_read_rtd = false;
    
    //rtd_raw = rtd;  // for monitoring
    
	rtd >>= 1;
    
    //rtd_remove_fault = rtd; // for monitoring
    
	return rtd;
}

void  Max31865_init(uint8_t numwires, uint8_t filterHz){
    SPI_DESELECT;
    
    //HAL_Delay(100);
    //SPI_SELECT;
    Max31865_setWires(numwires);
	Max31865_enableBias(MAX31865_CONFIG_DISABLE);
	Max31865_autoConvert(MAX31865_CONFIG_DISABLE);
	Max31865_clearFault();
    Max31865_setFilter(filterHz);
    Max31865_setThresholds(MAX31865_CONFIG_LOW_THRESHOLD, MAX31865_CONFIG_HIGH_THRESHOLD);
	//SPI_DESELECT;

	Kernel_Delay_Set(eTID_TEMPERATURE_TEST, 100);
}


bool Max31865_request_read_temp(){

	uint8_t buffer;
    //uint16_t rtd;
    
	
	Max31865_clearFault();
	Max31865_enableBias(MAX31865_CONFIG_ENABLE);
	
    buffer = Max31865_readRegister8(MAX31856_CONFIG_REG);
    
	buffer |= MAX31856_CONFIG_1SHOT;
	Max31865_writeRegister8(MAX31856_CONFIG_REG, buffer);
    
    return true;
}


bool Max31865_readTempC(float *readTemp){

    bool is_ok = false;
    float Z1, Z2, Z3, Z4, Rt, rpoly, temp;

	//SPI_SELECT;
    Rt = Max31865_readRTD();

        
	Rt /= 32768;
	Rt *= MAX31865_RREF;
	Z1 = -RTD_A;
	Z2 = RTD_A * RTD_A - (4 * RTD_B);
	Z3 = (4 * RTD_B) / MAX31865_RNOMINAL;
	Z4 = 2 * RTD_B;
	temp = Z2 + (Z3 * Rt);
	temp = (sqrtf(temp) + Z1) / Z4;

	if (temp >= 0){
        *readTemp = temp; 
        
        if(Max31865_readFault() == 0){
            is_ok= true;        
        }
        //SPI_DESELECT;
        return is_ok;
    }
    
	Rt /= MAX31865_RNOMINAL;
	Rt *= 100;    
	rpoly = Rt;
	temp = -242.02;
	temp += 2.2228 * rpoly;
	rpoly *= Rt;  // square
	temp += 2.5859e-3 * rpoly;
	rpoly *= Rt;  // ^3
	temp -= 4.8260e-6 * rpoly;
	rpoly *= Rt;  // ^4
	temp -= 2.8183e-8 * rpoly;
	rpoly *= Rt;  // ^5
	temp += 1.5243e-10 * rpoly;

    *readTemp = temp; 
    

    if(Max31865_readFault() == 0){
        is_ok = true;
    }

	//SPI_DESELECT;
            
    return is_ok;  
}