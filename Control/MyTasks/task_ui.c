#include "task_basic.h"
#include "ssd1322.h"



u8 test_selected_depth = 0;

char disp_msg_1st[20] = {0};
char disp_msg_2nd[20] = {0};


static void Main_menu_control(u16 btn);
static void Init_Test_Item(u8 item);


extern u8		frame_buffer[8192];
extern test_item_t* test_item_active;
extern test_item_t test_item[eTEST_MAX];






void Task_Disp_Test(u32 param){

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

            //disp_msg_1st, disp_msg_2nd are read-only due to thread-safe

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

        if(Receive_Msg_Single(eTID_GPIO_EXIT_TEST, &msg) == 0){
            btn = (u16)msg;
        }    
        else {
            btn = NO_MSG_SYMBOL;
        }   


        if(test_selected_depth == 0){

            Main_menu_control(btn);

            if(test_selected_depth == 1){
                Init_Test_Item(test_item_active->test_id);
                
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

        case BTN_CODE_FUNC1:
            PRINT_DBG("[%s] btn clean\r\n", __func__);
            break;

        case BTN_CODE_FUNC2:
            PRINT_DBG("[%s] btn cook\r\n", __func__);
            break;
        
    }
}


static void Init_Test_Item(u8 item){

    switch(item){

        case eTEST_PWM_DMA:     Task_Unblock(eTID_PWM_GEN_TEST, REPEAT_DISABLE); break;
        case eTEST_CLOSE_SENS:  Task_Unblock(eTID_GPIO_INPUT_TEST, REPEAT_ENABLE);   break;
        case eTEST_TEMPERATURE: Task_Unblock(eTID_TEMPERATURE_TEST, REPEAT_ENABLE);  break;
        
        case eTEST_REMOTE_COMM: Task_Unblock(eTID_REMOTE_CTL_TEST, REPEAT_DISABLE);       break;
        case eTEST_EEPROM_RW:   Task_Unblock(eTID_EEPROM_TEST, REPEAT_DISABLE);  break;

        case eTEST_GPIO_CTL:    Task_Unblock(eTID_GPIO_OUT_TEST, REPEAT_DISABLE);        break;
        case eTEST_ADC_CHECK:   Task_Unblock(eTID_MCU_ADC_TEST, REPEAT_ENABLE);     break;
        default:  test_selected_depth = 0;  return;
    }

    memset(disp_msg_1st, 0 ,20);
    memset(disp_msg_2nd, 0 ,20);

}




void HAL_GPIO_EXTI_Callback(u16 GPIO_Pin){

    if(Task_Get_Run_State(eTID_GPIO_EXIT_TEST) == T_HOLD){

        Task_Unblock(eTID_GPIO_EXIT_TEST, REPEAT_DISABLE);
        Send_Msg_Single(eTID_GPIO_EXIT_TEST, (u32)GPIO_Pin);
        
    }
    
}
