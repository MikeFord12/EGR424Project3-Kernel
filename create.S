/*
 * Implement the thread creation task:
 *
 *   - initialize the jump buffer with appropriate values for
 *     R13 (stack) and R14 (first address to jump to)
 *   - all other registers are irrelevant upon thread creation
 *
 *   In the jump buffer, the R13 slot is set to the second parameter of this
 *   function (the top-of-stack address, passed in R1). The R14 slot is set to
 *   the address of the threadStarter() function.
 *
 *   The C prototype for this function call is:
 *       createThread(threads[i].state, threads[i].stack)
 *   thus:
 *       R0 <-- state (a setjmp()-style jump buffer)
 *       R1 <-- stack (address of top-of-stack)
 */
    .syntax unified
    .text
    .align 2
    .thumb
    .thumb_func
    .type createThread,function
    .global createThread
createThread:  

	ldr r2, [r1]
	
	movw r3, 0x0000
	movt r3, 0x0100
	
	stmdb r2!, {r3}
	
	ldr r3, .L0
	stmdb r2!, {r3}
	
	@Initialize r0-r3, r12, and r14
	mov r3, #0
	mov r12, #0
	stmdb r2!, {r3}
	stmdb r2!, {r3, r12}
	stmdb r2!, {r3}
	stmdb r2!, {r3, r12}
	@save the pointer to the stack pointer
	str r2, [r1]
	@Store r4-r12 and PSP on array
	stm r0, {r2, r4-r12}
	bx lr

    
.L0:
    .word   threadStarter
