#ifndef _COMM_H__
#define _COMM_H__

#include <stdint.h>
#include "port_gpio.h"
#include "ModelConfig.h"


#define PORT_CAN			0
#define PORT_UART			1
#define PORT_WIFI			2
#define PORT_USB			3



#define COMM_QUEUE_USE		0
#define COMM_QUEUE_NOT_USE	1
#define COMM_MAX_QUEUE		5
#define COMM_BUF_SIZE		128
#define DISP_BUF_SIZE		48

#define SERIAL_CMD_NONE			0xFF


#define SERIAL_FORM_HEADER		0x02
#define SERIAL_FORM_ID			0xA7
#define SERIAL_FORM_TAIL		0x1F



#define DEVICE_SIGN			0x80



#define BTN_BUF_LEN		64
#define DISP_TXT_LEN	20
#define SERIAL_CMD_VALIDATE		0x10
#define SERIAL_CMD_CALIBRATION	0x20
#define SERIAL_CMD_UNIT_TEST	0x30



#define BTN_DISP_STX	0x02
#define BTN_DISP_ID		SERIAL_FORM_ID
#define BTN_DISP_TAIL	SERIAL_FORM_TAIL


#define BTN_CODE_ENTER	(uint16_t)(0x01 << 15)
#define BTN_CODE_MODE	(uint16_t)(0x01 << 5)
#define BTN_CODE_CLEAN	(uint16_t)(0x01 << 6)
#define BTN_CODE_COOK	(uint16_t)(0x01 << 8)

#define BTN_CODE_LEFT	(uint16_t)(0x01 << 9)
#define BTN_CODE_RIGHT	(uint16_t)(0x01 << 12)
#define BTN_CODE_UP		(uint16_t)(0x01 << 13)
#define BTN_CODE_DOWN	(uint16_t)(0x01 << 14)


#define BTN_LED_COOK	(uint16_t)0x0001
#define BTN_LED_CLEAN	(uint16_t)0x0002
#define BTN_LED_AUTO	(uint16_t)0x0004
#define BTN_LED_MANUAL	(uint16_t)0x0008
#define BTN_LED_ENTER	(uint16_t)0x0010


#define EANBLE_BTN_LED(X)	(X)
#define DISABLE_BTN_LED(X)	(!X)




typedef enum tag_uart_fsm
{
	Uart_WaitSync,
	Uart_WaitTgtId,
	Uart_WaitSrcId,
	Uart_WaitCmd,
	Uart_WaitSubCmd,
	Uart_WaitPara,
	Uart_WaitLength,
	Uart_WaitData,
	Uart_WaitCS,
	Uart_WaitTail,
	Uart_MaxState
}e_uart_slave_fsm_t;




#pragma pack(1)
typedef struct tag_comm_data {
	uint8_t id;
	uint8_t cmd;
	uint8_t subCmd;
	uint16_t leng;
	uint8_t cs;
	uint8_t validate;
	uint8_t port;
	uint8_t payload[COMM_BUF_SIZE];
}Comm_data_t;
#pragma pack()





#pragma pack(1)
typedef struct tag_DispBtn_data {
	uint8_t id;
	uint8_t cmd;
	uint8_t subCmd;
	uint16_t leng;
	uint8_t cs;
	uint8_t validate;
	uint8_t payload[DISP_BUF_SIZE];
}HostSerialData_t;
#pragma pack()




extern HostSerialData_t 	host_data_t;
extern uint8_t btnUartData;



int8_t Send_Packet(u8 port, uint8_t cmd, uint8_t subCmd, uint16_t size, const u8* payload);
int8_t Send_Packet_formated(u8 port, Comm_data_t* sendPkt);

int8_t CAN_comm_data_parsing(uint8_t* buf, uint8_t len, Comm_data_t* msg);
void Init_CAN_comm_state(uint16_t* rxCnt, Comm_data_t* dataTemp);


void Host_PC_uart_RxCb();
void Host_serial_data_parsing(uint8_t buf);
void Host_serial_data_init(uint16_t* rxCnt, HostSerialData_t* dataTemp);
void Send_data_to_host(HostSerialData_t* inData);



uint8_t CalcChecksum(uint8_t *data, uint8_t leng);





#endif

