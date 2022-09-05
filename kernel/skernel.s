

    RSEG    CODE:CODE(2)
	thumb

    EXTERN tcb_ready_item
    EXTERN tcb_prior_list
    EXTERN Kernel_Select_TCB
    EXTERN flag_prior_task

    PUBLIC PendSV_Handler
    PUBLIC SVC_Handler
    PUBLIC Kernel_Start_First_Task
    PUBLIC Kernel_Enable_VFP



PendSV_Handler:
    MRS 	R0,     PSP	
    ISB	

    /* ****************************** */
    // LDR     R1,      =flag_prior_task
    // LDR     R2,      [R1]

    // TST     R2,     #0x01
    // ITE     GE
    // LDRGE   R3,     =tcb_prior_list	
    // LDRLT   R3,     =tcb_ready_list	
    /* ****************************** */
                                                  
    LDR		R3,     =tcb_ready_item		/* Get the location of the current TCB. */
    LDR		R2,     [R3]			     
                                                   
    TST 	R14, #0x10			/* Is the task using the FPU context?  If so, push high vfp registers. */
    IT 		EQ	
    VSTMDBEQ  R0!, {S16-S31}
    //VSTMDB  R0!, {S16-S31}	


    STMDB   R0!,    {R4-R11, R14}	   /* Save the core registers. */
    STR 	R0, 	[R2]		          /* Save the new top of stack into the first member of the TCB. */
                                      
    STMDB   SP!,    {R0, R3}		    
    MOV 	R0,     #0 				      
    MSR 	BASEPRI,R0			       
    DSB								           
    ISB								            
    BL 		Kernel_Select_TCB			     
    MOV 	R0,     #0				      
    MSR 	BASEPRI,R0			        
    LDMIA   SP!,    {R0, R3}		    
                                      
    LDR 	R1,     [R3]			       /* The first item in pxCurrentTCB is the task top of stack. */
    LDR 	R0,     [R1]			      
                                      
    LDMIA   R0!,    {R4-R11, R14}	  /* Pop the core registers. */
                                      
    TST 		R14, #0x10			/* Is the task using the FPU context?  If so, pop the high vfp registers too. */
    IT 		EQ						
    VLDMIAEQ R0!, {S16-S31}			
    //VLDMIA  R0!, {S16-S31}

    MSR 	PSP,    R0				     
    ISB				


                                      
    BX 		R14				          



SVC_Handler:
    CPSID I							/* Globally disable interrupts. */

    LDR		R3,     =tcb_ready_item		 /* Restore the context. */
    LDR 	R1,     [R3]			     /* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
    LDR 	R0,     [R1]			     /* The first item in pxCurrentTCB is the task top of stack. */
    LDMIA   R0!,    {R4-r11, R14}	 /* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
    MSR 	PSP,    R0				     /* Restore the task stack pointer. R0 points stack pointer of CurrentTCB   */
    ISB								          
    CPSIE   I					 /* Globally enable interrupts. */
    MOV 	R0,     #0 				    
    MSR		BASEPRI,R0				    
    BX 		R14						        
   									


Kernel_Start_First_Task:
    LDR 	R0,     =0xE000ED08 		/* Use the NVIC offset register to locate the stack. */
    LDR 	R0,     [R0] 				// R0 = Vector Table Offset
    LDR 	R0,     [R0] 				// R0 = *(Vector Table Offset)
    MSR 	MSP,    R0					/* Set the msp back to the start of the stack. */
    MOV 	R0,     #0					/* Clear the bit that indicates the FPU is in use, see comment above. */
    MSR 	CONTROL,R0			
    CPSIE I							/* Globally enable interrupts. */
    CPSIE F					
    DSB						
    ISB						
    SVC 0								/* System call to start first task. */
        



Kernel_Enable_VFP:
	/* The FPU enable bits are in the CPACR. */
	ldr.w r0, =0xE000ED88
	ldr	r1, [r0]

	/* Enable CP10 and CP11 coprocessors, then save back. */
	orr	r1, r1, #( 0xf << 20 )
	str r1, [r0]
    DSB
    ISB
	bx	r14
	END