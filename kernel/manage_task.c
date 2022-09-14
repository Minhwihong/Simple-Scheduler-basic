#include "manage_task.h"
#include "taskDef.h"

u32 single_msg_que[eTID_MAX][MSG_QUEUE_LEN] = {0};
u16 single_msg_que_sts[eTID_MAX] = {0};


extern tcb_t tcb_list[eTID_MAX];
extern tcb_t tcb_super_list[eSuperId_Max];

extern tcb_t* tcb_ready_item;
extern tcb_t* tcb_prior_item;
extern tcb_t* tcb_ready_bak;

extern u8 flag_prior_task ;




void Task_Init(task_stack_t* tasks, task_stack_t* tasks_super ){

    for(u8 idx=0; idx<eTID_MAX; ++idx){

        Kernel_Add_TCB(tcb_list, tasks[idx].id, &tasks[idx], T_RUN, (void*)0);
    }

    for(u8 idx=0; idx<eSuperId_Max; ++idx){

        Kernel_Add_TCB(tcb_super_list, tasks_super[idx].id, &tasks_super[idx], T_RUN, (void*)0);
    }
}




void Task_Unblock(u8 id, u8 repeat){

    tcb_list[id].runflag = T_RUN;
    tcb_list[id].repeat = repeat;
    tcb_list[id].first_run = 1;
}


void Task_Block(u8 id){

    tcb_list[id].runflag = T_HOLD;
}

u8 Task_Get_Run_State(u8 id){



    return tcb_list[id].runflag;
}



u8 Task_Start_Ctl(u8 id){

    if(tcb_list[id].repeat == REPEAT_DISABLE){

        if(tcb_list[id].first_run > 0){
            tcb_list[id].first_run = 0;
            return 0;
        }
        else {
            tcb_list[id].runflag = T_HOLD;
            
            //Task yield
            TASK_YIELD();
            //*(u32 volatile *)0xE000ED04 |= (1U << 28);
            return 1;
        }
       
    }
    else if(tcb_list[id].repeat == REPEAT_ENABLE){

        if(tcb_list[id].period > 0 && tcb_list[id].first_run == 0){
            Kernel_Delay_Set(id, tcb_list[id].period);
        }
        else {
            tcb_list[id].first_run = 0;
        }
        
    }

    return 0;
}




signed char Active_Super_Task(unsigned char id){

    CRITICAL_SECTION_IN();

    signed char rtn = Kernel_Manage_SuperTask(PRIOR_TCB_ADD, &tcb_super_list[id]);

    if(tcb_prior_item != NULL){
        flag_prior_task = PRIOR_TASK_START;
    }

    CRITICAL_SECTION_OUT();
    TASK_YIELD();

    return rtn;
}




signed char Deactive_Super_Task(unsigned char id){

    CRITICAL_SECTION_IN();

    signed char rtn = Kernel_Manage_SuperTask(PRIOR_TCB_REMOVE, &tcb_super_list[id]);

    if(tcb_prior_item == NULL){
        flag_prior_task = PRIOR_TASK_END;
    }

    CRITICAL_SECTION_OUT();
    TASK_YIELD();

    return rtn;
}



signed char Receive_Msg_Single(unsigned char id,  u32* pm){

    unsigned short queue_sts = 0;

    if(single_msg_que_sts[id] == 0){
        return 1;
    }

    queue_sts = single_msg_que_sts[id];

    *pm =  single_msg_que[id][queue_sts-1];
    single_msg_que_sts[id] = single_msg_que_sts[id] - 1;

     return 0;
}


signed char Send_Msg_Single(unsigned char id,  u32 pm){

     unsigned short* queue_sts = (void*)0;

    if(single_msg_que_sts[id] == MSG_QUEUE_LEN || id > eTID_MAX){
        return 1;
    }

    queue_sts = &single_msg_que_sts[id];

    single_msg_que[id][*queue_sts] 			= pm;

    (*queue_sts)++;
          
    return 0;
}
