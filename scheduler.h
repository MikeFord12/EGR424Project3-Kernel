#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <setjmp.h>
#include "inc/lm3s6965.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

// thread_t is a pointer to function with no parameters and
// no return value...i.e., a user-space thread.
typedef void (*thread_t)(void);

// These are the external user-space threads. In this program, we create
// the threads statically by placing their function addresses in
// threadTable[]. A more realistic kernel will allow dynamic creation
// and termination of threads.
extern void UART_Thread1(void);
extern void UART_Thread2(void);
extern void OLED_Thread(void);
extern void LED_Thread(void);

#define STACK_SIZE 4096   // Amount of stack space for each thread
#define NUM_THREADS (sizeof(threadTable)/sizeof(threadTable[0]))

static thread_t threadTable[] = {
        UART_Thread1,
        UART_Thread2,
        OLED_Thread,
        LED_Thread
};

typedef struct threadLock
{
        unsigned lock_state;
        unsigned lock_count;
        int lock_owner;
}lock_t;

typedef struct {
        int active; // non-zero means thread is allowed to run
        char *stack; // pointer to TOP of stack (highest memory location)
        unsigned savedRegs[40]; //Save R4-R11, R13, and LR in exception handler
} threadStruct_t;

// These static global variables are used in scheduler(), in
// the yield() function, and in threadStarter()
static jmp_buf scheduler_buf;   // saves the state of the scheduler
static threadStruct_t threads[NUM_THREADS]; // the thread table

unsigned currThread; // The currently active thread
void yield(void);
unsigned lock_acquire(lock_t* lock);
void lock_release(lock_t* lock);
void SystickInit();
void InitializeLED(void);
void InitializePeripherals(void);
int save_registers(unsigned* buffer);
void restore_registers(unsigned* buffer);
void lock_release(lock_t* lock);
unsigned lock_acquire(lock_t* lock);
void lock_init(lock_t* lock);



#define LED0_REG (HWREG(0x40025000 + 0x004))

lock_t UART_LOCK;

#endif //_SCHEDULER_H_
