#include "taskDef.h"

#define VALVE_OPEN      1
#define VALVE_CLOSE     0




enum e_valves {
    e_port_1,
    e_port_2,
    e_port_3,
    e_port_4,
    e_port_5,
    e_vlv_max,
};




char valve_name[e_vlv_max][24] = {
    "Port 1\0",
    "Port 2\0",
    "Port 3\0",
    "Port 4\0",
    "Port 5\0",
};

typedef struct gpio_out_ctl_tag{
    u8 state;
    void (*Control)(struct gpio_out_ctl_tag* ptr_gpio_ctl, u8 ctl);
    char* name;
}gpio_out_t;




extern char disp_msg_1st[20];
extern char disp_msg_2nd[20];
extern u8 test_selected_depth;




static void Ball_valve1_open_ctl(gpio_out_t* ptr_valve, u8 ctl);
static void Ball_valve1_close_ctl(gpio_out_t* ptr_valve, u8 ctl);
static void Ball_valve2_open_ctl(gpio_out_t* ptr_valve, u8 ctl);
static void Ball_valve2_close_ctl(gpio_out_t* ptr_valve, u8 ctl);
static void Solenoid_valve_ctl(gpio_out_t* ptr_valve, u8 ctl);




void Task_GPIO_Out_Test(uint32_t param){

    gpio_out_t valve_ctl[e_vlv_max];
    u8 IsInit_valve_ctl = 0;
    u8 selected_valve = e_port_1;

    u32 msg = 0;
    u16 btn = 0;

    valve_ctl[e_port_1].Control     = Ball_valve1_open_ctl;
    valve_ctl[e_port_2].Control    = Ball_valve1_close_ctl;
    valve_ctl[e_port_3].Control     = Ball_valve2_open_ctl;
    valve_ctl[e_port_4].Control    = Ball_valve2_close_ctl;
    valve_ctl[e_port_5].Control           = Solenoid_valve_ctl;

    for(u8 idx=0; idx<e_vlv_max; ++idx) {
        valve_ctl[idx].state = VALVE_CLOSE;
        valve_ctl[idx].name = valve_name[idx];
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
                selected_valve++;
                if(selected_valve == e_vlv_max){
                    selected_valve = e_port_1;
                }

                PRINT_DBG("[%s] valve selcect - [%d]%s \r\n", __func__, selected_valve, valve_ctl[selected_valve].name);
                break;

            case BTN_CODE_DOWN:
                if(selected_valve == 0){
                    selected_valve = e_port_5;
                }
                else {
                    selected_valve--;
                }

                PRINT_DBG("[%s] valve selcect - [%d]%s \r\n", __func__, selected_valve, valve_ctl[selected_valve].name);
                break;

            case BTN_CODE_LEFT:
                valve_ctl[selected_valve].Control(&valve_ctl[selected_valve], VALVE_CLOSE);
                PRINT_DBG("[%s] Valve State %d\r\n", __func__,  valve_ctl[selected_valve].state);
                break;

            case BTN_CODE_RIGHT:
                valve_ctl[selected_valve].Control(&valve_ctl[selected_valve], VALVE_OPEN);
                break;

            case BTN_CODE_ENTER:
                break;

            case BTN_CODE_CLEAN:
                test_selected_depth--;
                PRINT_DBG("[%s] exit EEPROM test\r\n", __func__);
                break;

            default:
                break;
            
        }

        //disp_msg_1st

        sprintf(disp_msg_1st, "%s", valve_ctl[selected_valve].name);
        sprintf(disp_msg_2nd, "Valve %s",valve_ctl[selected_valve].state == 1 ? "Open" : "Close" );



    }

}






void Task_MCU_ADC_Test(uint32_t param){

    while(1){

        if(Task_Start_Ctl(CURR_TASK_ID) > 0){
            continue;
        } 

        
    }
    
}






static void Ball_valve1_open_ctl(gpio_out_t* ptr_valve, u8 ctl){

    if(ctl == VALVE_OPEN){
        BALL_VALVE_01_OPEN(GPIO_PIN_SET);
        ptr_valve->state = VALVE_OPEN;
    }
    else {
        BALL_VALVE_01_OPEN(GPIO_PIN_RESET);
        ptr_valve->state = VALVE_CLOSE;
    }

}

static void Ball_valve1_close_ctl(gpio_out_t* ptr_valve, u8 ctl){
    if(ctl == VALVE_OPEN){
        BALL_VALVE_01_CLOSE(GPIO_PIN_SET);
        ptr_valve->state = VALVE_OPEN;
    }
    else {
        BALL_VALVE_01_CLOSE(GPIO_PIN_RESET);
        ptr_valve->state = VALVE_CLOSE;
    }
}


static void Ball_valve2_open_ctl(gpio_out_t* ptr_valve, u8 ctl){
     if(ctl == VALVE_OPEN){
        BALL_VALVE_02_OPEN(GPIO_PIN_SET);
        ptr_valve->state = VALVE_OPEN;
    }
    else {
        BALL_VALVE_02_OPEN(GPIO_PIN_RESET);
        ptr_valve->state = VALVE_CLOSE;
    }
}

static void Ball_valve2_close_ctl(gpio_out_t* ptr_valve, u8 ctl){
    if(ctl == VALVE_OPEN){
        BALL_VALVE_02_CLOSE(GPIO_PIN_SET);
        ptr_valve->state = VALVE_OPEN;
    }
    else {
        BALL_VALVE_02_CLOSE(GPIO_PIN_RESET);
        ptr_valve->state = VALVE_CLOSE;
    }
}


static void Solenoid_valve_ctl(gpio_out_t* ptr_valve, u8 ctl){
    if(ctl == VALVE_OPEN){
        SOL_VALVE_CTL(GPIO_PIN_SET);
        ptr_valve->state = VALVE_OPEN;
    }
    else {
        SOL_VALVE_CTL(GPIO_PIN_RESET); 
        ptr_valve->state = VALVE_CLOSE;
    }
}


