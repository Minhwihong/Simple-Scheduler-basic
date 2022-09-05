#include "taskDef.h"

static u8 console_rcv_byte = 0;
static char console_msg[32] = {0};
static u16 console_msg_len = 0;
static u8 console_msg_flag = 0;




extern u8 test_selected_depth;
extern char disp_msg_1st[20];
extern char disp_msg_2nd[20];



static void Console_port_rcv_hnd(u8 ch);





void Task_MCU_LED_Test(u32 param){

    while(1){

        if(Task_Start_Ctl(CURR_TASK_ID) > 0){
            continue;
        } 

        HAL_GPIO_TogglePin(MCU_LED1_GPIO_Port, MCU_LED1_Pin);
    }
}



void Task_EEPROM_Test(u32 param){

    while(1){



    }


}






signed char Task_Console(u8 id, u16 param){

    char msg[64];

    if(console_msg_flag){

        memcpy(msg, console_msg, 64);
        memset(console_msg, 0, 64);
        console_msg_len = 0;
        console_msg_flag = 0;

        
    }

    return 0;
}


void Console_port_init(){

    console_msg_len = 0;
     HAL_UART_Receive_IT(&UART_DBG, &console_rcv_byte, 1);

}



void Console_Rx_Cb(){

    u8 ch = 0;

    ch = console_rcv_byte;
    HAL_UART_Receive_IT(&UART_DBG, &console_rcv_byte, 1);
    Console_port_rcv_hnd(ch);
}



static void Console_port_rcv_hnd(u8 ch){

    if(console_msg_flag){
        return;
    }

    if(ch != '\r'){

        console_msg[console_msg_len++] = ch;
    }
    else {
        console_msg_flag = 1;
    }
}