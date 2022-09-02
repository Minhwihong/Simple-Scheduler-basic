#include "taskDef.h"

uint32_t xPrescale[50] 	
	//  1    	2   	3    	4   	5    	6    	7   	8  		9   	10
	= { 420, 	280, 	210, 	168, 	140, 	120, 	105,  	93, 	84, 	76, 
		70,  	64, 	60,  	56, 	53,  	49, 	46,  	44, 	42, 	40,
		38,  	37, 	36,  	35, 	34,  	33, 	32,  	31, 	30, 	29, 
		28,  	27, 	26,  	25, 	24,  	23, 	22,  	21, 	20, 	19,
		18,  	17, 	16,  	15, 	14};

u32 cover_pwm_end_cnt = 0;
u32 sideL_pwm_end_cnt = 0;
u32 sideR_pwm_end_cnt = 0;


extern u8 test_selected_depth;

extern char disp_msg_1st[20];
extern char disp_msg_2nd[20];

uint32_t pwm_buf[128] = {0};
uint32_t pwm_buf_2[128] = {0};


static void Set_cover_motor_dir(bool dir);


void Task_PWM_Gen_Test(uint32_t param){

    u32 msg = 0;
    u16 btn = 0;

    u8 pwm_start = 0;
    u8 spd_idx = 8;
    BOOL dir = FALSE;


    while(1){

        if(Task_Start_Ctl(CURR_TASK_ID) > 0){
            continue;
        } 

        if(Receive_Msg_Single(eID_PWM_GEN_TEST, &msg) == 0){
            btn = (u16)msg;
        }    
        else {
            btn = NO_MSG_SYMBOL;
        }   

        switch(btn){

            case BTN_CODE_UP:
                spd_idx++;
                spd_idx = spd_idx%40;

                PWM_TIMER_COVER.Instance->PSC = xPrescale[spd_idx];
                PRINT_DBG("[%s] spd idx:%02d, PSC:%03d\r\n", __func__,  spd_idx, xPrescale[spd_idx]);
                break;

            case BTN_CODE_DOWN:

                if(spd_idx == 0){
                    spd_idx = 40-1;
                }
                else {
                    spd_idx--;
                }

                PWM_TIMER_COVER.Instance->PSC = xPrescale[spd_idx];
                PRINT_DBG("[%s] spd idx:%02d, PSC:%03d\r\n", __func__,  spd_idx, xPrescale[spd_idx]);
                break;

            case BTN_CODE_LEFT:     dir = FALSE;    break;
            case BTN_CODE_RIGHT:    dir = TRUE;     break;

            case BTN_CODE_MODE:     break;

            case BTN_CODE_ENTER:
                
                if(pwm_start == 1){
                    for(u16 idx=0; idx<128; ++idx){
                        pwm_buf[idx] = 49;
                    }

                    Set_cover_motor_dir(dir);  
                    //Set_cover_motor_enable(true);

                    PWM_TIMER_COVER.Instance->CCR3 = 49;
                    HAL_TIM_PWM_Start_DMA(&PWM_TIMER_COVER, PWM_CHANNEL_COVER, pwm_buf, 128);
                    PRINT_DBG("[%s] Cover Opener PWM starts\r\n", __func__);
                }
                else {
                    
                    HAL_TIM_PWM_Stop_DMA(&PWM_TIMER_COVER, PWM_CHANNEL_COVER);
                    //Set_cover_motor_enable(false);
                    PRINT_DBG("[%s] Cover Opener PWM Stops\r\n", __func__);
                }

                pwm_start++;
                pwm_start = pwm_start%2;
                break;

            case BTN_CODE_CLEAN:
                HAL_TIM_PWM_Stop_DMA(&PWM_TIMER_COVER, PWM_CHANNEL_COVER);
                //Set_cover_motor_enable(false);
                pwm_start           = 0;
                spd_idx             = 8;
                cover_pwm_end_cnt   = 0;
                test_selected_depth--;
                PRINT_DBG("[%s] exit Cover Opener test\r\n", __func__);
                break;

            default:
                break;


        }

        sprintf(disp_msg_1st,  "Cover Opener %02d %c", cover_pwm_end_cnt, dir == TRUE ? 'O' : 'C');
        sprintf(disp_msg_2nd, "%s, spd:%d", pwm_start == 0 ? "Stop" : "Start", xPrescale[spd_idx]);

    }

}








void Task_GPIO_Input_Task(uint32_t param){

    u32 msg = 0;
    u16 btn = 0;

    u8 sens_coverR_lock = 0;
    u8 sens_coverR_open = 0;
    u8 sens_coverL_lock = 0;
    u8 sens_coverL_open = 0;
    u8 sens_coverT_lock = 0;
    u8 sens_coverT_open = 0;

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

            case BTN_CODE_UP:       break;
            case BTN_CODE_DOWN:     break;
            case BTN_CODE_LEFT:     break;
            case BTN_CODE_RIGHT:    break;
            case BTN_CODE_MODE:     break;
            case BTN_CODE_ENTER:    break;

            case BTN_CODE_CLEAN:
                test_selected_depth--;
                Task_Block(CURR_TASK_ID);
                PRINT_DBG("[%s] exit Cover Opener test\r\n", __func__);
                break;

            case TASK_INIT_SYMBOL:  break;
            default:                break;

        }


        sens_coverR_lock = READ_COVER_RIGHT_LOCK;
        sens_coverR_open = READ_COVER_RIGHT_OPEN;
        sens_coverL_lock = READ_COVER_LEFT_LOCK;
        sens_coverL_open = READ_COVER_LEFT_OPEN;
        sens_coverT_lock = READ_COVER_TILT_LOCK;
        sens_coverT_open = READ_COVER_TILT_OPEN;


        sprintf(disp_msg_1st, "%s", "RL RO LL LO TL TO");
        sprintf(disp_msg_2nd, "%d  %d  %d  %d  %d  %d",
            sens_coverR_lock, sens_coverR_open, sens_coverL_lock, sens_coverL_open, sens_coverT_lock, sens_coverT_open);

        
    }
}








void Task_PWM_Manage(uint32_t param){

    while(1){

        if(cover_pwm_end_cnt < 20){
			HAL_TIM_PWM_Start_DMA(&PWM_TIMER_COVER, PWM_CHANNEL_COVER, pwm_buf, 128);
		}
		else {
			cover_pwm_end_cnt = 0;
            HAL_TIM_PWM_Stop_DMA(&PWM_TIMER_COVER, PWM_CHANNEL_COVER);
		}

        //HAL_GPIO_TogglePin(GPIOA, TEST3_Pin); 

        PRINT_DBG("[%s] super task\r\n", __func__);

        Deactive_Super_Task(CURR_TASK_ID);
    }


}







static void Set_cover_motor_dir(bool dir){
	//TIM3_CH4_SM 
	if(dir == true){
		HAL_GPIO_WritePin(GPIOx_COVER_DR_L, GPIO_PIN_COVER_DR_L, GPIO_PIN_RESET); 
		HAL_GPIO_WritePin(GPIOx_COVER_DR_R, GPIO_PIN_COVER_DR_R, GPIO_PIN_SET); 
	}
	else{
		HAL_GPIO_WritePin(GPIOx_COVER_DR_L, GPIO_PIN_COVER_DR_L, GPIO_PIN_SET); 
		HAL_GPIO_WritePin(GPIOx_COVER_DR_R, GPIO_PIN_COVER_DR_R, GPIO_PIN_RESET); 
	}
		
}