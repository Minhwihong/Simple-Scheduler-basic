#include "main.h"
#include "port_gpio.h"
#include "myQueue.h"



e_uart_slave_fsm_t serial_msg_fsm = Uart_WaitSync;
e_uart_slave_fsm_t host_msg_fsm = Uart_WaitSync;
e_uart_slave_fsm_t CAN_msg_fsm = Uart_WaitSync;



u8 UseSendQueue = FALSE;


extern UART_HandleTypeDef huart1;


#if 0

int8_t Send_Packet(u8 port, uint8_t cmd, uint8_t subCmd, uint16_t size, const u8* payload){

	Comm_data_t sendPkt;

	if(UseSendQueue == TRUE){
		
		sendPkt.cmd 	= cmd;
		sendPkt.subCmd 	= subCmd;
		sendPkt.leng 	= size;

		if(payload != NULL){
			for(u8 idx=0; idx<size; ++idx){
				sendPkt.payload[idx] = payload[idx];
			}
		}

		sendPkt.port 	= port;

		switch(port){
			case PORT_CAN:	CommQueue_push_back(&cSend_CAN_Queue, &sendPkt);	break;
			case PORT_UART:	break;
			case PORT_USB:	break;
		}
	}
	else {

		switch(port){
			case PORT_CAN:	sendDataByCAN(cmd, subCmd, size, payload);	break;
			case PORT_UART:	break;
			case PORT_USB:	break;
		}
	}

	return 1;
}


int8_t Send_Packet_formated(u8 port, Comm_data_t* sendPkt){

	sendPkt->port = port;
	
	if(UseSendQueue == TRUE){
		
		switch(port){
			case PORT_CAN:	CommQueue_push_back(&cSend_CAN_Queue, sendPkt);	break;
			case PORT_UART:	break;
			case PORT_USB:	break;
		}
	}
	else {
		
		switch(port){
			case PORT_CAN:	SendDataByCAN_formated(sendPkt);	break;
			case PORT_UART:	break;
			case PORT_USB:	break;
		}
	}

	return 1;
	
}


int8_t Use_Send_Queue(u8 use){

//	UseSendQueue = use;
//
//	if(UseSendQueue == TRUE){
//
//		if(Task_Run_State(ID_SEND_MSG) == false){
//			Task_Restart(ID_SEND_MSG, REPEAT_ENABLE, 0);
//		}
//
//		 Commuse_single_queue(&cSend_CAN_Queue, COMM_QUEUE_NOT_USE);
//	}
//	else {
//		
//		if(Task_Run_State(ID_SEND_MSG) == T_RUN){
//			Task_Stop(ID_SEND_MSG);
//		}
//
//		Commuse_single_queue(&cSend_CAN_Queue, COMM_QUEUE_USE);
//	}
	
	return 1;
	
}




int8_t CAN_comm_data_parsing(uint8_t* buf, uint8_t len, Comm_data_t* msg){

	static uint16_t 	rxCount 		= 0;
	static uint8_t 		lengCnt 		= 0;
	static uint16_t 	dataCnt 		= 0;
	static uint8_t 		lengBuf[2] 		= {0};
	static Comm_data_t 	CAN_dataTemp;

	uint16_t 			lengTmp 		= 0;


	for(uint8_t idx=0; idx<len; ++idx){

		switch(CAN_msg_fsm)
		{
			case Uart_WaitSync: 	/* HEAD */
				rxCount++;
				if (buf[idx] == SERIAL_FORM_HEADER){

					CAN_msg_fsm			= Uart_WaitSrcId;
				}
				else{

					CAN_msg_fsm			= Uart_WaitSync;
					rxCount 			= 0;
					Init_CAN_comm_state(&rxCount, &CAN_dataTemp);
				}
				break;

			case Uart_WaitSrcId:	/* ID */

				rxCount++;
				CAN_dataTemp.id = buf[idx];

				if (buf[idx] == SERIAL_FORM_ID){

					CAN_msg_fsm			= Uart_WaitCmd;
				}
				else{

					CAN_msg_fsm			= Uart_WaitSync;
					rxCount 			= 0;
					Init_CAN_comm_state(&rxCount, &CAN_dataTemp);
				}
				break;

			case Uart_WaitCmd:		/* CMD */
				rxCount++;

				CAN_dataTemp.cmd	= buf[idx];
				CAN_msg_fsm			= Uart_WaitSubCmd;
				break;

			case Uart_WaitSubCmd:		/* CMD */
				rxCount++;

				CAN_dataTemp.subCmd	= buf[idx];
				CAN_msg_fsm			= Uart_WaitLength;
				break;

			case Uart_WaitLength:	/* DATA Length */
				rxCount++;
				if(lengCnt == 0){

					lengBuf[0] = buf[idx];
					lengCnt++;
				}
				else if(lengCnt == 1){

					lengBuf[1] = buf[idx];

					memcpy(&lengTmp, lengBuf, sizeof(uint16_t));
					lengCnt = 0;

					if (lengTmp == 0){ 	/* No Payload */ 

						CAN_dataTemp.leng 		= 0;
						CAN_msg_fsm				= Uart_WaitCS;
					}
					else{

						CAN_dataTemp.
						leng 		= lengTmp;
						CAN_msg_fsm				= Uart_WaitData;
					}
				}
				break;

			case Uart_WaitData: 	/* DATA */
				rxCount++;
				CAN_dataTemp.payload[dataCnt++] = buf[idx];

				if (dataCnt >= CAN_dataTemp.leng){

					CAN_msg_fsm 		= Uart_WaitCS;
					dataCnt 			= 0;
				}

				if (CAN_dataTemp.leng > 100){

					CAN_dataTemp.cmd	= SERIAL_CMD_NONE;
					CAN_dataTemp.leng 	= dataCnt = 0;
					CAN_msg_fsm			= Uart_WaitSync;
					rxCount 			= 0;
					Init_CAN_comm_state(&rxCount, &CAN_dataTemp);
				}
				break;

			case Uart_WaitCS:		/* CS */

				rxCount++;
				CAN_dataTemp.cs			= buf[idx];
				CAN_msg_fsm				= Uart_WaitTail;
				break;

			case Uart_WaitTail: 	/* TAIL */

				rxCount++;

				if (buf[idx] == SERIAL_FORM_TAIL){
					CAN_dataTemp.validate = 1;

					msg->cmd 				= CAN_dataTemp.cmd;
					msg->subCmd 			= CAN_dataTemp.subCmd;
					msg->id 				= CAN_dataTemp.id;
					msg->leng 				= CAN_dataTemp.leng;
					msg->validate 			= CAN_dataTemp.validate;
					msg->cs 				= CAN_dataTemp.cs;
					msg->port 				= PORT_CAN;
					
					memcpy(&msg->payload[0], &CAN_dataTemp.payload[0], COMM_BUF_SIZE);

					
					CAN_dataTemp.validate 	= 0;
					CAN_dataTemp.cmd		= SERIAL_CMD_NONE;
					CAN_dataTemp.leng 		= rxCount = 0;
					CAN_msg_fsm				= Uart_WaitSync;
					
					Init_CAN_comm_state(&rxCount, &CAN_dataTemp);
					return 0;
				}

				CAN_dataTemp.validate 		= 0;
				CAN_dataTemp.cmd			= SERIAL_CMD_NONE;
				CAN_dataTemp.leng 			= rxCount = 0;
				CAN_msg_fsm					= Uart_WaitSync;
				Init_CAN_comm_state(&rxCount, &CAN_dataTemp);
				break;

			default:
				break;
		}

	}


	return 1;

}





void Init_CAN_comm_state(uint16_t* rxCnt, Comm_data_t* dataTemp){
	//serialOffset = 0;
	CAN_msg_fsm			= Uart_WaitSync;
	*rxCnt 				= 0;
	dataTemp->cmd = SERIAL_CMD_NONE;
	dataTemp->validate = 0;
}




void Host_PC_uart_RxCb(){
	HAL_UART_Receive_IT(&UART_MES, &btnUartData, 1);
	Host_serial_data_parsing(btnUartData);

		
}




void Host_serial_data_parsing(uint8_t buf){
	
	static uint16_t rxCount 	= 0;
	static uint8_t lengCnt 		= 0;
	static uint16_t dataCnt 	= 0;
	static uint8_t lengBuf[2] 	= {0};
	
	static HostSerialData_t uart_dataTemp;

	uint16_t 	lengTmp = 0;



	switch(host_msg_fsm)
	{
		case Uart_WaitSync: 	/* HEAD */
			rxCount++;
			if (buf == BTN_DISP_STX)
			{
				host_msg_fsm			= Uart_WaitSrcId;
			}
			else
			{
				host_msg_fsm			= Uart_WaitSync;
				rxCount 			= 0;
				Host_serial_data_init(&rxCount, &uart_dataTemp);
			}
			break;

		case Uart_WaitSrcId:	/* ID */
			rxCount++;
			uart_dataTemp.id = buf;
			if (buf == BTN_DISP_ID)
			{
				host_msg_fsm			= Uart_WaitCmd;
			}
			else
			{
				host_msg_fsm			= Uart_WaitSync;
				rxCount 			= 0;
				Host_serial_data_init(&rxCount, &uart_dataTemp);
			}
			break;

		case Uart_WaitCmd:		/* CMD */
			rxCount++;

			uart_dataTemp.cmd		= buf;
			host_msg_fsm			= Uart_WaitSubCmd;
			break;

		case Uart_WaitSubCmd:		/* CMD */
			rxCount++;

			uart_dataTemp.subCmd	= buf;
			host_msg_fsm			= Uart_WaitLength;
			break;

		case Uart_WaitLength:	/* DATA Length */
			rxCount++;
			if(lengCnt == 0){
				memset(lengBuf, 0, 2);
				lengBuf[0] = buf;
				lengCnt++;
			}
			else if(lengCnt == 1){
				lengBuf[1] = buf;

				memcpy(&lengTmp, lengBuf, sizeof(uint16_t));
				lengCnt = 0;

				if (lengTmp == 0) /* No Payload */
				{
					uart_dataTemp.leng		= 0;
					host_msg_fsm				= Uart_WaitCS;
				}
				else
				{
					uart_dataTemp.leng		= lengTmp;
					host_msg_fsm				= Uart_WaitData;
				}
			}
			break;

		case Uart_WaitData: 	/* DATA */
			rxCount++;
			uart_dataTemp.payload[dataCnt++] = buf;

			if (dataCnt >= uart_dataTemp.leng)
			{
				host_msg_fsm = Uart_WaitCS;
				dataCnt 	= 0;
			}

			if (uart_dataTemp.leng > 100)
			{
				uart_dataTemp.cmd			= SERIAL_CMD_NONE;
				uart_dataTemp.leng	= dataCnt = 0;
				host_msg_fsm			= Uart_WaitSync;
				rxCount 				= 0;
				Host_serial_data_init(&rxCount, &uart_dataTemp);
			}
			break;

		case Uart_WaitCS:		/* CS */
			rxCount++;
			uart_dataTemp.cs				= buf;
			host_msg_fsm				= Uart_WaitTail;
			break;

		case Uart_WaitTail: 	/* TAIL */
			rxCount++;
			if (buf == BTN_DISP_TAIL)
			{
				uart_dataTemp.validate = 1;

				host_data_t.cmd		= uart_dataTemp.cmd;
				host_data_t.subCmd 	= uart_dataTemp.subCmd;
				host_data_t.id 		= uart_dataTemp.id;
				host_data_t.leng	= uart_dataTemp.leng;
				host_data_t.validate  = uart_dataTemp.validate;
				host_data_t.cs 		= uart_dataTemp.cs;
				
				memcpy(&host_data_t.payload[0], &uart_dataTemp.payload[0], DISP_BUF_SIZE);

				
				uart_dataTemp.validate		= 0;
				uart_dataTemp.cmd			= SERIAL_CMD_NONE;
				uart_dataTemp.leng			= rxCount = 0;
				host_msg_fsm				= Uart_WaitSync;
				
				Host_serial_data_init(&rxCount, &uart_dataTemp);
			}

			uart_dataTemp.validate		= 0;
			uart_dataTemp.cmd			= SERIAL_CMD_NONE;
			uart_dataTemp.leng			= rxCount = 0;
			host_msg_fsm					= Uart_WaitSync;
			Host_serial_data_init(&rxCount, &uart_dataTemp);
			break;

		default:
			break;
	}



}




void Host_serial_data_init(uint16_t* rxCnt, HostSerialData_t* dataTemp){
	//serialOffset = 0;
	host_msg_fsm		= Uart_WaitSync;
	*rxCnt 				= 0;
	dataTemp->cmd = SERIAL_CMD_NONE;
	dataTemp->validate = 0;
}




void Send_data_to_host(HostSerialData_t* inData){

	u8 sendBuf[BTN_BUF_LEN] = {0};
	u16 add = 0;
	u16 len = inData->leng;

	sendBuf[add++] = BTN_DISP_STX;
	sendBuf[add++] = BTN_DISP_ID;
	sendBuf[add++] = inData->cmd;
	sendBuf[add++] = inData->subCmd;

	memcpy(&sendBuf[add], &len, sizeof(u16));
	add += sizeof(u16);

	if(len > 0){
		for(uint16_t idx=0; idx<inData->leng; ++idx){
			sendBuf[add++] = inData->payload[idx];
		}
	}
	

	//checksum cal
	sendBuf[add++] = 0;
	sendBuf[add++] = BTN_DISP_TAIL;

	//send uart
	HAL_UART_Transmit(&UART_MES, sendBuf, add, 100);
	
}






uint8_t CalcChecksum(uint8_t *data, uint8_t leng)
{
  uint8_t csum;

    csum = 0xFF;
    for (;leng > 0;leng--)
        csum += *data++;
    return ~csum;
}



#endif









