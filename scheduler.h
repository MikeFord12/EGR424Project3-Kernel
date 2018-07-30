#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <setjmp.h>
#include "inc/lm3s6965.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

typedef struct threadLock
{
        unsigned lock_state;
        unsigned lock_count;
        int lock_owner;
}lock_t;

extern unsigned currThread;
extern void yield(void);
extern unsigned lock_acquire(lock_t* lock);
extern void lock_release(lock_t* lock);



#define LED0_REG (HWREG(0x40025000 + 0x004))



lock_t UART_LOCK;
#endif //_SCHEDULER_H_
