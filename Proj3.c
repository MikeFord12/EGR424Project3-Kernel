/*--------------------------------------------------------------------------------
   -- Date: 07/30/2018
   -- Project Name: EGR-424-Project-3
   -- Description: Multi-Threaded Kernel
   -- Names: Jason Hunter, Mike Ford, Brandon Baars
   ----------------------------------------------------------------------------------*/
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "rit128x96x4.h"
#include "scheduler.h"
#include "driverlib/timer.h"
#include "inc/hw_ints.h"


void main(void)
{
        unsigned i;
        int j;

        //Initialize the currThread to -1 to start
        currThread = -1;

        //Initialize UART lock
        lock_init(&UART_LOCK);

        InitializePeripherals();

        InitializeLED();
		
		//IntMasterEnable();
		SystickInit();
		IntMasterEnable();
		
		GPIOPinWrite(5,1,1);
		
        // Create all the threads and allocate a stack for each one
        for (i=0; i < NUM_THREADS; i++) {
                // Mark thread as runnable
                threads[i].active = 1;

                // Allocate stack
                threads[i].stack = (char *)malloc(STACK_SIZE) + STACK_SIZE;
                if (threads[i].stack == 0) {
                        iprintf("Out of memory\r\n");
                        exit(1);
                }

                // After createThread() executes, we can execute a longjmp()
                // to threads[i].state and the thread will begin execution
                // at threadStarter() with its own stack.
                createThread(threads[i].savedRegs, &(threads[i].stack));


        }

        // Start running coroutines
        yield();

        // If scheduler() returns, all coroutines are inactive and we return
        // from main() hence exit() should be called implicitly (according to
        // ANSI C). However, TI's startup_gcc.c code (ResetISR) does not
        // call exit() so we do it manually.
        exit(0);
}


/*****************************************************************************
   // This function is called from within user thread context. It executes
   // a SVC instruction
*****************************************************************************/
void yield(void)
{
        //    iprintf("HERE 3\r\n");
        asm volatile ("svc #100");

}

/*****************************************************************************
//This function initializes the SysTick to generate a 1ms interrupt. 
//This interrupt is what will be used as a pre-emptive kernel. 
//After the interrupt fires, the scheduler will take over and revoke access to the thread, thus allowing another thread to do work. 
*****************************************************************************/
void SystickInit()
{
        NVIC_ST_CTRL_R  =  0;
        NVIC_ST_RELOAD_R =  0x00001F40;
        NVIC_ST_CURRENT_R  =  0;
       /* NVIC_ST_CTRL_R = NVIC_ST_CTRL_CLK_SRC |
                         NVIC_ST_CTRL_INTEN |
                         NVIC_ST_CTRL_ENABLE; */
	    NVIC_ST_CTRL_R    |=  0x00000007;

}


/*****************************************************************************
   // *Initializes LED on boared and pin to measure context switch time
*****************************************************************************/
void InitializeLED(void){
        // Initialize Pins for LED
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
        GPIO_PORTF_DIR_R = 0x01;
        GPIO_PORTF_DEN_R = 0x01;
		
		//Initialize Port E pin 3
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
		GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3);
		GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_3,0xFF);
}

/*****************************************************************************
   // Initializes peripherals such as UART 
*****************************************************************************/
void InitializePeripherals(void)
{

        // Set the clocking to run directly from the crystal.
        SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_8MHZ);

        // Initialize the OLED display and write status.
        RIT128x96x4Init(1000000);


        // Enable the peripherals used by this example.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

        // Set GPIO A0 and A1 as UART pins.
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        // Configure the UART for 115,200, 8-N-1 operation.
        UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
							 

}


// This is the starting point for all threads. It runs in user thread
// context using the thread-specific stack. The address of this function
// is saved by createThread() in the LR field of the jump buffer so that
// the first time the scheduler
/*****************************************************************************
   // starts threads
*****************************************************************************/
void threadStarter(void)
{
        // Call the entry point for this thread. The next line returns
        // only when the thread exits.
        (*(threadTable[currThread]))();

        // Do thread-specific cleanup tasks. Currently, this just means marking
        // the thread as inactive. Do NOT free the stack here because we're
        // still using it! Remember, this function runs in user thread context.
        threads[currThread].active = 0;

        // This yield returns to the scheduler and never returns back since
        // the scheduler identifies the thread as inactive.
        yield();
}

// This is the "main loop" of the program.
// This handler gets called either by an SVC call or the systick timer interrupt
/*****************************************************************************
   // Scheduler, executed after SVC call or 1ms
*****************************************************************************/
void scheduler_handler(void)
{
		//Toggle pin to measure context switch time
		GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_3,0xFF);

        // Save current thread state if not the first time through
        if(currThread != -1)
        {
                save_registers(threads[currThread].savedRegs);
        }
        // Loop through all of the active threads
        do
        {
                if (++currThread >= NUM_THREADS)
                {
                        currThread = 0;
                }
        } while (threads[currThread].active != 1);

		GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_3,0x00);

        // Restore the thread state for the thread about to be executed
        // This jumps to the next thread
        restore_registers(threads[currThread].savedRegs);
}

/*****************************************************************************
   // saves the threads state and fakes a return
*****************************************************************************/
int save_registers(unsigned* buffer)
{

        asm volatile ("mrs r1,psp\n"
                      "stm r0, {r1, r4-r12}");

        return 0;

}

/*****************************************************************************
   // Restores register and fakes a return 
*****************************************************************************/
void restore_registers(unsigned* buffer)
{
        asm volatile ("ldr r1, [r0] \n"
                      "add r0, r0, #4\n"
                      "msr psp, r1\n"
                      "ldm r0, {r4-r12}\n"
                      "movw lr, 0xfffd\n"
                      "movt lr, 0xffff\n"
                      "bx lr");
}

/*****************************************************************************
   // Releases lock of thread
*****************************************************************************/
void lock_release(lock_t* lock)
{
        lock->lock_count--; // decrement lock count

        if(lock->lock_count == 0 && lock->lock_owner == currThread)
        {
                // really unlocked, release the lock
                lock->lock_owner = 999;
                lock->lock_state = 1;
        }
}

/*****************************************************************************
   // Acquires the lock for thread to use
*****************************************************************************/
unsigned lock_acquire(lock_t* lock)
{
        if(lock->lock_state == 1)
        {
                // If already locked, increment lock count
                lock->lock_count++;
                lock->lock_state = 0;
                lock->lock_owner = currThread;
                return 1;
        }
        else
                return 0;

}

/*****************************************************************************
   // Initializes Lock
*****************************************************************************/
void lock_init(lock_t* lock)
{
        lock->lock_state = 1; // initialize lock as released
        lock->lock_count = 0; // initialize no lock
        lock->lock_owner = -1; // not an owner

}


/*****************************************************************************
   // Instruction to tell processor to go into sleep mode
*****************************************************************************/
void enter_sleep_mode()
{
        asm volatile ("WFI");
}

/*
 * Compile with:
 * ${CC} -o crsched.elf -I${STELLARISWARE} -L${STELLARISWARE}/driverlib/gcc
 *     -Tlinkscript.x -Wl,-Map,crsched.map -Wl,--entry,ResetISR
 *     crsched.c create.S threads.c startup_gcc.c syscalls.c rit128x96x4.c
 *     -ldriver
 */
// vim: expandtab ts=2 sw=2 cindent
