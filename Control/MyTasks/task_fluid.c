#include "taskDef.h"




enum e_valves {
    e_port_1,
    e_port_2,
    e_port_max,
};



char port_name[e_port_max][24] = {
    "Port 1\0",
    "Port 2\0",
};

typedef struct gpio_out_ctl_tag{
    u8 state;
    void (*Control)(struct gpio_out_ctl_tag* ptr_self, u8 ctl);
    char* name;
}gpio_out_t;




extern char disp_msg_1st[20];
extern char disp_msg_2nd[20];
extern u8 test_selected_depth;




static void GPIO_Output_Ctl_1(gpio_out_t* ptr_self, u8 ctl);
static void GPIO_Output_Ctl_2(gpio_out_t* ptr_self, u8 ctl);





void Task_GPIO_Out_Test(uint32_t param){

    gpio_out_t port_ctl[e_port_max];
    u8 selected_port = e_port_1;

    u32 msg = 0;
    u16 btn = 0;

    port_ctl[e_port_1].Control  = GPIO_Output_Ctl_1;
    port_ctl[e_port_2].Control  = GPIO_Output_Ctl_2;

    for(u8 idx=0; idx<e_port_max; ++idx) {
        port_ctl[idx].state = 0;
        port_ctl[idx].name = port_name[idx];
    }

    while(1){

        if(Task_Start_Ctl(CURR_TASK_ID) > 0){
            continue;
        } 

        if(Receive_Msg_Single(CURR_TASK_ID, &msg) == 0){
            btn = (u16)msg;
        }    
        else {
            btn = NO_MSG_SYMBOL;
        }  


        switch(btn){

            case BTN_CODE_UP:
                selected_port++;
                if(selected_port == e_port_max){
                    selected_port = e_port_1;
                }

                PRINT_DBG("[%s] port select - [%d]%s \r\n", __func__, selected_port, port_ctl[selected_port].name);
                break;

            case BTN_CODE_DOWN:
                if(selected_port == 0){
                    selected_port = e_port_2;
                }
                else {
                    selected_port--;
                }

                PRINT_DBG("[%s] port select - [%d]%s \r\n", __func__, selected_port, port_ctl[selected_port].name);
                break;

            case BTN_CODE_LEFT:
                port_ctl[selected_port].Control(&port_ctl[selected_port], 0);
                PRINT_DBG("[%s] port State %d\r\n", __func__,  port_ctl[selected_port].state);
                break;

            case BTN_CODE_RIGHT:
                port_ctl[selected_port].Control(&port_ctl[selected_port], 1);
                break;

            case BTN_CODE_ENTER:
                break;

            case BTN_CODE_FUNC1:
                test_selected_depth--;
                PRINT_DBG("[%s] Exit GPIO Output test\r\n", __func__);
                break;

            default:
                break;
            
        }

        //disp_msg_1st

        sprintf(disp_msg_1st, "%s", port_ctl[selected_port].name);
        sprintf(disp_msg_2nd, "Valve %s",port_ctl[selected_port].state == 1 ? "Open" : "Close" );



    }

}






void Task_MCU_ADC_Test(u32 param){

    while(1){

        if(Task_Start_Ctl(CURR_TASK_ID) > 0){
            continue;
        } 

        
    }
    
}






static void GPIO_Output_Ctl_1(gpio_out_t* ptr_self, u8 ctl){

    if(ctl == 1){
        GPIO_OUTPUT_CTL01(GPIO_PIN_SET);
        ptr_self->state = 1;
    }
    else {
        GPIO_OUTPUT_CTL01(GPIO_PIN_RESET);
        ptr_self->state = 0;
    }

}

static void GPIO_Output_Ctl_2(gpio_out_t* ptr_self, u8 ctl){
    if(ctl == 1){
        GPIO_OUTPUT_CTL02(GPIO_PIN_SET);
        ptr_self->state = 1;
    }
    else {
        GPIO_OUTPUT_CTL02(GPIO_PIN_RESET);
        ptr_self->state = 0;
    }
}


