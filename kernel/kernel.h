#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdint.h>
#include "ModelConfig.h"


#define MAX_STACK_SIZE      (4024*3)

#define SET_PEND_SV         (*(uint32_t volatile *)0xE000ED04 |= (1U << 28))

#define CRITICAL_SECTION_IN     __set_BASEPRI(0xFF)
#define CRITICAL_SECTION_OUT    __set_BASEPRI(0)


#define PRIOR_TCB_ADD     0
#define PRIOR_TCB_REMOVE  1


#define portYIELD()                                 \
{                                                   \
    /* Set a PendSV to request a context switch. */ \
    (*(uint32_t volatile *)0xE000ED04 |= (1U << 28)); \
    __DSB();                                        \
    __ISB();                                        \
}


#define PRIOR_TASK_START        1
#define PRIOR_TASK_END          2
#define PRIOR_TASK_NORMAL       0





typedef struct {
    volatile uint32_t r0;
    volatile uint32_t r1;
    volatile uint32_t r2;
    volatile uint32_t r3;
    volatile uint32_t r12;
    volatile uint32_t lr;
    volatile uint32_t pc;
    volatile uint32_t xpsr;
} OS_SystemCtx_t;



typedef struct {
    //uint32_t lr;
	volatile uint32_t r4;
	volatile uint32_t r5;
	volatile uint32_t r6;
	volatile uint32_t r7;
	volatile uint32_t r8;
	volatile uint32_t r9;
	volatile uint32_t r10;
	volatile uint32_t r11;
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




typedef int8_t *(*OS_Func_t)(uint32_t pm);


typedef struct task_stack_tag{
    uint8_t id;
    uint32_t* stackStart;
    uint32_t  stackSize;

    void* param;
    void (* execfunc)(uint32_t pm);

}task_stack_t;


typedef struct tcb_tag{
    
  volatile uint32_t* sp;

  volatile  task_stack_t* task;

  uint8_t runflag;
  uint8_t repeat;
  uint8_t first_run;
  uint32_t period;

  uint8_t is_in_delay;
  uint32_t curr_tick;
  uint32_t match_tick;

  struct tcb_tag *tcb_next;  

}tcb_t;




void KernelLaunch(void);    
void KernelInit(void);    
int8_t AddThreadToTCB(tcb_t* ptr_tcb, volatile  uint8_t id, volatile  task_stack_t* task_stack_ptr, uint8_t run_flag, void* args);
void Scheduling(void);    
void StartFirstTask( void );


void Make_TCB_List(void);
void KernelStackInit(void);

void Clear_task_delay(uint8_t id);
void Set_task_delay(uint8_t id, uint32_t tick);

int8_t Update_Tcb_prior_list(uint8_t mode, tcb_t* tcb);

void SelectNextTCB(void);


void  osSchedulerLaunch(void);
void vPortEnableVFP( void );
void OS_task_exit_handle(void);



#endif