#include "taskDef.h"
#include "ssd1322.h"




test_item_t test_item[e_test_Max] = {
    {"Cover Open\0",    eTest_cover_open,   NULL,   NULL},
    {"Cover Lock\0",    eTest_cover_lock,   NULL,   NULL},
    {"Limit Sensor\0",  eTest_limit_sens,   NULL,   NULL},
    {"Temperature\0",   eTest_temperature,  NULL,   NULL},
    {"Induction Ctl\0", eTest_Induction,    NULL,   NULL},
    {"CAN Comm.\0",     eTest_CAN_comm,     NULL,   NULL},
    {"MP3 Player\0",    eTest_MP3_player,   NULL,   NULL},
    {"EEPROM R/W\0",    eTest_EEPROM_RW,    NULL,   NULL},
    {"Valve Ctl\0",     eTest_valve,        NULL,   NULL},
    {"Oil Pump Ctl\0",  eTest_oil_pump,     NULL,   NULL},
    {"Pressure Check\0",eTest_pressure,     NULL,   NULL},
    {"Fan Check\0",     eTest_fan,          NULL,   NULL},

};

u8 test_item_no = 0;
u8 test_selected_item = 0;
u8 test_selected_depth = 0;
test_item_t* test_item_active = NULL;

char disp_msg_1st[20] = {0};
char disp_msg_2nd[20] = {0};


static void Main_menu_control(u16 btn);
static void Init_test_item_disp(u8 item);
//static void Test_menu_Btn_control(u16 btn);


extern u8		frame_buffer[8192];



void Task_Disp_Test(uint32_t param){

    while(1){

        if(Task_Start_Ctl(CURR_TASK_ID) > 0){
            continue;
        }

        ssd1322_fill_fb(frame_buffer, 0x00);

        if(test_selected_depth == 0){

            ssd1322_put_string_fb(frame_buffer, 0, 0, " ");
            ssd1322_put_string_fb(frame_buffer, 4, 0, "Select Item");
            ssd1322_put_string_fb(frame_buffer, 0, 36, " ");
            ssd1322_put_string_fb(frame_buffer, 4, 36, test_item_active->test_name);
            
        }
        else if(test_selected_depth == 1){

            ssd1322_put_string_fb(frame_buffer, 0, 0, " ");
            ssd1322_put_string_fb(frame_buffer, 4, 0, disp_msg_1st);

            ssd1322_put_string_fb(frame_buffer, 0, 36, " ");
            ssd1322_put_string_fb(frame_buffer, 4, 36, disp_msg_2nd);
        }

        ssd1322_display_fb(frame_buffer); 

        //OLED SPI refresh consume time : ~65ms
    }

}





void Task_Gpio_Exit_Test(u32 param){

    u32 msg = 0;
    
    while(1){

        u16     btn = 0;
        u8      task_id = 0;

        if(Task_Start_Ctl(CURR_TASK_ID) > 0){
            continue;
        }     

        if(Receive_Msg_Single(eID_GPIO_EXIT_TEST, &msg) == 0){
            btn = (u16)msg;
        }    
        else {
            btn = NO_MSG_SYMBOL;
        }   


        if(test_selected_depth == 0){

            Main_menu_control(btn);

            if(test_selected_depth == 1){
                Init_test_item_disp(test_item_active->test_id);
                
            }
                
        }
        else if(test_selected_depth == 1){

            // First, Send Btn Code Message
            Send_Msg_Single( Get_TaskId_From_TestItem(test_item_active->test_id), msg);

            // In case of Blocked Task unblock the task
            task_id = Get_TaskId_From_TestItem(test_item_active->test_id);

            if(task_id != eId_UNVALID_TASK && Task_Get_Run_State(task_id) == T_HOLD){
                Task_Unblock(task_id, REPEAT_DISABLE);
            }

        }


        //Set_task_delay(eID_BUTTON_TEST, 250);
    }
}







static void Main_menu_control(u16 btn){

    switch(btn){

        case  BTN_CODE_DOWN:
            test_item_active = test_item_active->next_item;
            Buzzor_Start(500);
            PRINT_DBG("[%s] btn down\r\n", __func__);
            break;

        case BTN_CODE_UP:
            test_item_active = test_item_active->prev_item;
            Buzzor_Start(500);
            PRINT_DBG("[%s] btn up\r\n", __func__);
            break;

        case BTN_CODE_LEFT:
            PRINT_DBG("[%s] btn left\r\n", __func__);
            break;

        case BTN_CODE_RIGHT:
            PRINT_DBG("[%s] btn right\r\n", __func__);
            break;

        case BTN_CODE_ENTER:
            test_selected_depth++;
            test_selected_depth = test_selected_depth%2;
            Buzzor_Start(500);
            PRINT_DBG("[%s] btn enter\r\n", __func__);
            break;

        case BTN_CODE_MODE:
            PRINT_DBG("[%s] btn mode\r\n", __func__);
            break;

        case BTN_CODE_CLEAN:
            PRINT_DBG("[%s] btn clean\r\n", __func__);
            break;

        case BTN_CODE_COOK:
            PRINT_DBG("[%s] btn cook\r\n", __func__);
            break;
        
    }
}


static void Init_test_item_disp(u8 item){

    switch(item){

        case eTest_cover_open:  Task_Unblock(eID_PWM_GEN_TEST, REPEAT_DISABLE); break;
        case eTest_valve:       Task_Unblock(eID_GPIO_OUT_TEST, REPEAT_DISABLE);        break;
        case eTest_limit_sens:  Task_Unblock(eID_GPIO_INPUT_TEST, REPEAT_ENABLE);   break;
        case eTest_temperature: Task_Unblock(eID_TEMPERATURE_TEST, REPEAT_ENABLE);  break;
        case eTest_pressure:    Task_Unblock(eID_MCU_ADC_TEST, REPEAT_ENABLE);     break;
        default:  test_selected_depth = 0;  return;
    }

    memset(disp_msg_1st, 0 ,20);
    memset(disp_msg_2nd, 0 ,20);

}


void Init_test_item_list(){

    test_item[0].next_item = &test_item[1];
    test_item[0].prev_item = &test_item[e_test_Max-1];

    for(u8 idx=1; idx<e_test_Max-1; ++idx){
        test_item[idx].next_item = &test_item[idx+1];
        test_item[idx].prev_item = &test_item[idx-1];
    }

    test_item[e_test_Max-1].next_item = &test_item[0];
    test_item[e_test_Max-1].prev_item = &test_item[e_test_Max-2];

    test_item_active = &test_item[4];

}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

    if(Task_Get_Run_State(eID_GPIO_EXIT_TEST) == T_HOLD){

        Task_Unblock(eID_GPIO_EXIT_TEST, REPEAT_DISABLE);
        Send_Msg_Single(eID_GPIO_EXIT_TEST, (uint32_t)GPIO_Pin);
        
    }
    
}
