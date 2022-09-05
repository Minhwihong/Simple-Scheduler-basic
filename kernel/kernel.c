#include "kernel.h"
#include "myTasks.h"
#include "main.h"

#define portFPCCR          ( ( volatile u32 * ) 0xe000ef34 ) /* Floating point context control register. */
#define portASPEN_AND_LSPEN_BITS   ( 0x3UL << 30UL )

extern u32 arr_stack[MAX_STACK_SIZE] ;


tcb_t* tcb_curr = NULL;


tcb_t tcb_list[eTID_MAX];
tcb_t tcb_super_list[eSuperId_Max];


tcb_t* tcb_ready_item = NULL;
tcb_t* tcb_prior_item = NULL;
tcb_t* tcb_ready_bak = NULL;

u32 flag_prior_task = PRIOR_TASK_NORMAL;


volatile u32 curr_stk_offset = 0;
static u8 kernel_start = 0;

void Kernel_Init(){
  
  Kernel_Make_TCB_List();
  
#if(__GNUC__)  
//  vPortEnableVFP();
#endif  

  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 15);
  
}




/* ***********************************************
System Stack Frame layout
  struct {
      uint32_t r0;        // Low Addr  (0x0000)
      uint32_t r1;
      uint32_t r2;
      uint32_t r3;
      uint32_t r12;
      uint32_t lr;
      uint32_t pc;
      uint32_t xpsr;      // High Addr  (0x1000)	<- StackEnd (Top Of Stack)
  } OS_SystemCtx_t;
************************************************** */
int8_t Kernel_Add_TCB(tcb_t* ptr_tcb, volatile  u8 id,  volatile task_stack_t* task_stack_ptr, u8 run_flag, void* args){

	volatile u32* stackTop = NULL;

  //__set_BASEPRI(0xFF);

  if(curr_stk_offset + task_stack_ptr->stackSize > MAX_STACK_SIZE){
    return 1;
  }

	stackTop = &arr_stack[curr_stk_offset];
	stackTop += task_stack_ptr->stackSize;

	//stackTop = stackTop & ~((uint32_t)0x03);

    
  /* If you use FPU, must have FPU register space */
  stackTop -= 17;
  /* ********************************************* */

	stackTop--;
	*stackTop   = 0x01000000;						/* xPSR */

	stackTop--;
	*stackTop   = (u32)(task_stack_ptr->execfunc) & 0xfffffffeUL;	/* PC */

	stackTop--;

	//*stackTop = (uint32_t)&OS_task_exit_handle;	/* LR */
  //*stackTop   = (void*)0;	/* LR */


	stackTop    -= 5;                       /* R12, R3, R2 and R1. */
	*stackTop   = (u32)args; 				        /* R0 */

	/* A save method is being used that requires each task to maintain its
	* own exec return value. */
	stackTop--;
	*stackTop   = 0xfffffffd;

	stackTop    -= 8; 								/* R11, R10, R9, R8, R7, R6, R5 and R4. */



	ptr_tcb[id].sp         = stackTop;
	ptr_tcb[id].task       = task_stack_ptr;
	ptr_tcb[id].runflag    = run_flag;

  ptr_tcb[id].repeat     = REPEAT_ENABLE;
	

  ptr_tcb[id].curr_tick  = 0;
  ptr_tcb[id].match_tick = 0;


  curr_stk_offset         += task_stack_ptr->stackSize;

  //__set_BASEPRI(0);

  return 0;
}







void Kernel_Launch(){

  tcb_ready_item = &tcb_list[0];
  tcb_prior_item = NULL;

  tcb_curr = tcb_ready_item;
  kernel_start = 1;
  Kernel_Enable_VFP();
  
  *( portFPCCR ) |= portASPEN_AND_LSPEN_BITS;

}


void Kernel_Scheduling(void){
  

  for(uint8_t idx=0; idx<eTID_MAX; ++idx){

    if(tcb_list[idx].is_in_delay == 1){
      tcb_list[idx].curr_tick++;
    }

  }

  //*(uint32_t volatile *)0xE000ED04 |= (1U << 28);
  REQUEST_PEND_SV();
}



void Kernel_Select_TCB(void){

  tcb_t* lTcb_next;

  CRITICAL_SECTION_IN;

  switch(flag_prior_task){
    case PRIOR_TASK_START:
      flag_prior_task   = PRIOR_TASK_NORMAL;
      tcb_ready_bak     = tcb_ready_item;
      tcb_ready_item    = tcb_prior_item;
      break;

    case PRIOR_TASK_END:
      flag_prior_task   = PRIOR_TASK_NORMAL;
      tcb_ready_item    = tcb_ready_bak;
      break;

    default:
      break;
  }

  lTcb_next = tcb_ready_item->tcb_next;


  while(1){

    if(lTcb_next->runflag == T_RUN){

      /* ************** Check Delay State ************** */
      if(lTcb_next->is_in_delay == 1){

        if(lTcb_next->curr_tick >= lTcb_next->match_tick){

          lTcb_next->is_in_delay = 0;
          lTcb_next->curr_tick    = 0;
          lTcb_next->match_tick   = 0;
          tcb_ready_item = lTcb_next;
          break;
        }
        else {

          lTcb_next = lTcb_next->tcb_next;
        }

      /* ************** Check Delay State ************** */
      }
      else {

        tcb_ready_item = lTcb_next;
        break;
      }

      
    }
    else {

      lTcb_next = lTcb_next->tcb_next;
    }

  }

  tcb_curr = tcb_ready_item;

  CRITICAL_SECTION_OUT;

}





void Kernel_Delay_Set(u8 id, u32 tick){

  tcb_list[id].is_in_delay = 1;
  tcb_list[id].match_tick = tick;

  //*(uint32_t volatile *)0xE000ED04 |= (1U << 28);
  TASK_YIELD();
}

void Kernel_Delay_Clear(u8 id){

  tcb_list[id].is_in_delay = 0;
}



void Kernel_Make_TCB_List(){
  
  for(s8 idx=0; idx<eTID_MAX; ++idx){
    
    if(idx < eTID_MAX-1){
      tcb_list[idx].tcb_next = &tcb_list[idx+1];
    }
    else {
      tcb_list[idx].tcb_next = &tcb_list[0];
    }
  }

}







s8 Kernel_Manage_SuperTask(u8 mode, tcb_t* tcb){

  tcb_t* tcb_start = tcb_prior_item;

  switch(mode){

    case PRIOR_TCB_ADD:

      // add prior list
      if(tcb_prior_item == NULL){

        tcb_prior_item = tcb;
        tcb_prior_item->tcb_next = tcb_prior_item;
        return 0;
      }
      else {

        while(tcb_prior_item->tcb_next != tcb_start){

          tcb_prior_item = tcb_prior_item->tcb_next;
        }

        tcb->tcb_next = tcb_prior_item->tcb_next;
        tcb_prior_item->tcb_next = tcb;
        return 0;
      }
      break;

    case PRIOR_TCB_REMOVE:
      // delete prior list
      if(tcb_prior_item == NULL){
        return 1;
      }

      if(tcb_prior_item->tcb_next == tcb_start){

        if(tcb_prior_item == tcb){
          tcb_prior_item = NULL;
          return 0;
        }
        
        return 1;
      }
      else {

        while(tcb_prior_item->tcb_next != tcb_start && 
          tcb_prior_item->tcb_next != tcb){

          tcb_prior_item = tcb_prior_item->tcb_next;
        }

        if(tcb_prior_item->tcb_next != tcb_start){

          tcb_prior_item->tcb_next = tcb_prior_item->tcb_next->tcb_next;
          return 0;
        }

        return 1;

      }
      break;

    default:
      break;
  }

  return 2;
}





/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  if(kernel_start == 1)
    Kernel_Scheduling();
  /* USER CODE END SysTick_IRQn 1 */
}




#if 0
__attribute__( ( naked ) )
void vPortEnableVFP( void )
{
    __asm volatile
    (
    "	LDR.W 	R0, =0xE000ED88			\n" /* The FPU enable bits are in the CPACR. */
    "	LDR 		R1, [R0]					\n"
    "									\n"
    "	ORR 		R1, R1, #( 0xF << 20 )		\n" /* Enable CP10 and CP11 coprocessors, then save back. */
    "	STR 		R1, [R0]					\n"
    "	BX 		R14						\n"
    );
}
#endif

