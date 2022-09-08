#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdint.h>
#include "ModelConfig.h"


#define MAX_STACK_SIZE      (4024*3)

 

#define CRITICAL_SECTION_IN()     \
    {                           \
        __set_BASEPRI(0xFF);    \
    } 


#define CRITICAL_SECTION_OUT()  \
{                               \
     __set_BASEPRI(0);          \
}   


#define PRIOR_TCB_ADD     0
#define PRIOR_TCB_REMOVE  1



#define TASK_YIELD()                                 \
{                                                   \
    /* Set a PendSV to request a context switch. */ \
    (*(u32 volatile *)0xE000ED04 |= (1U << 28)); \
    __DSB();                                        \
    __ISB();                                        \
}

#define REQUEST_PEND_SV()   \
{               \
    (*(u32 volatile *)0xE000ED04 |= (1U << 28));          \
    __DSB();                                        \
    __ISB();                                        \
}


#define PRIOR_TASK_START        1
#define PRIOR_TASK_END          2
#define PRIOR_TASK_NORMAL       0





typedef struct {
    volatile u32 r0;
    volatile u32 r1;
    volatile u32 r2;
    volatile u32 r3;
    volatile u32 r12;
    volatile u32 lr;
    volatile u32 pc;
    volatile u32 xpsr;
} OS_SystemCtx_t;



typedef struct {
    //uint32_t lr;
	volatile u32 r4;
	volatile u32 r5;
	volatile u32 r6;
	volatile u32 r7;
	volatile u32 r8;
	volatile u32 r9;
	volatile u32 r10;
	volatile u32 r11;
#if USE_FPU
    uint32_t s16;
    uint32_t s17;
    uint32_t s18;
    uint32_t s19;
    uint32_t s20;
    uint32_t s21;
    uint32_t s22;
    uint32_t s23;
    uint32_t s24;
    uint32_t s25;
    uint32_t s26;
    uint32_t s27;
    uint32_t s28;
    uint32_t s29;
    uint32_t s30;
    uint32_t s31;
#endif
} OS_TaskCtx_t;




typedef s8 *(*OS_Func_t)(u32 pm);


typedef struct task_stack_tag{
    u8 id;
    u32* stackStart;
    u32  stackSize;

    void* param;
    void (* execfunc)(u32 pm);

}task_stack_t;


typedef struct tcb_tag{
    
  volatile u32* sp;

  volatile  task_stack_t* task;

  u8 runflag;
  u8 repeat;
  u8 first_run;
  u32 period;

  u8 is_in_delay;
  u32 curr_tick;
  u32 match_tick;

  struct tcb_tag *tcb_next;  

}tcb_t;




void Kernel_Launch(void);    
void Kernel_Init(void);    
int8_t Kernel_Add_TCB(tcb_t* ptr_tcb, volatile  u8 id, volatile  task_stack_t* task_stack_ptr, u8 run_flag, void* args);
void Kernel_Scheduling(void);    
void Kernel_Start_First_Task( void );


void Kernel_Make_TCB_List(void);
void Kernel_Delay_Clear(u8 id);
void Kernel_Delay_Set(u8 id, u32 tick);

int8_t Kernel_Manage_SuperTask(u8 mode, tcb_t* tcb);

void Kernel_Select_TCB(void);
void Kernel_Enable_VFP( void );

void OS_task_exit_handle(void);



#endif