#include "taskDef.h"
#include "max31865.h"

#define TEMP_MEAS_OK        0
#define TEMP_MEAS_ERR       1
#define TEMP_SPI_DEINIT      2
#define TEMP_SPI_INIT      3
#define TEMP_IC_INIT        4




extern char disp_msg_1st[20];
extern char disp_msg_2nd[20];
extern u8 test_selected_depth;



static void Temperature_Btn_Handle(u16 btn);

u8 ih_set_mode = 0;

static float g_temp = 0.0f;
static u8 g_temp_sts = 0;
static u8 g_temp_ok = 0;




void Task_Temperature_Test(u32 param){

    u32 msg = 0;
    float temp = 0;
    bool rtn = false;

    sprintf(disp_msg_1st, "%s", "Temperature");
    sprintf(disp_msg_2nd, "%s","Init...");
    Max31865_init(MAX31865_CONFIG_3WIRE, MAX31865_CONFIG_60HZ);

    while(1){

        if(Task_Start_Ctl(CURR_TASK_ID) > 0){
            continue;
        } 

        if(Receive_Msg_Single(eTID_TEMPERATURE_TEST, &msg) == 0){
           Temperature_Btn_Handle((u16)msg);
        }
        
        rtn = Max31865_readTempC(&temp);

        g_temp_ok = rtn;
        g_temp = temp;

        if(g_temp_ok == 0){

            g_temp_sts = TEMP_SPI_DEINIT;
            HAL_SPI_DeInit(&TEMP_SPI);
            HAL_SPI_MspDeInit(&TEMP_SPI);
            Kernel_Delay_Set(CURR_TASK_ID, 250);

            g_temp_sts = TEMP_SPI_INIT;
            MX_SPI4_Init();
            Kernel_Delay_Set(CURR_TASK_ID, 250);

            g_temp_sts = TEMP_IC_INIT;
            Max31865_init(MAX31865_CONFIG_3WIRE, MAX31865_CONFIG_60HZ);
            Kernel_Delay_Set(CURR_TASK_ID, 250);
        }
        else {
            g_temp_sts = TEMP_MEAS_OK;
            //g_temp = temp;
        }


        memset(disp_msg_1st, 0 ,20);
        memset(disp_msg_2nd, 0 ,20);

        sprintf(disp_msg_1st, "temp read - %s", rtn == true ? "OK" : "Err");
        sprintf(disp_msg_2nd, "temp : %3.2foC",temp);

        HAL_GPIO_TogglePin(GPIOA, TEST3_Pin); 
    }  
 
}








static void Temperature_Btn_Handle(u16 btn){
    switch(btn){

        case BTN_CODE_FUNC1:
            test_selected_depth--;
            Task_Block(eTID_TEMPERATURE_TEST);
            PRINT_DBG("[%s] Exit Temperature test\r\n", __func__);
            break;

        case TASK_INIT_SYMBOL:
        default:
            break;
    }
}


