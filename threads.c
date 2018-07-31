#include <stdio.h>
#include "scheduler.h"

// These are the user-space threads. Note that they are completely oblivious
// to the technical concerns of the scheduler. The only interface to the
// scheduler is the single function yield() and the global variable
// currThread which indicates the number of the thread currently
// running.
/*****************************************************************************
// *Insert Comment*
*****************************************************************************/
void UART_Thread1(void)
{
        unsigned count;
        int i;

        while (1) {
                //aquire lock
                if(lock_acquire(&UART_LOCK))
                {
                        for (count = 0; count < 10; count++) {
                                for(i=0; i<50000; i++);
                                iprintf("In thread %u -- pass %d\r\n", currThread, count);
                        }
                }
                //release lock
                lock_release(&UART_LOCK);
                yield();
        }
}

/*****************************************************************************
// *Insert Comment*
*****************************************************************************/
void UART_Thread2(void)
{
        unsigned count;
        int i;

        while (1) {
                //aquire lock
                if(lock_acquire(&UART_LOCK))
                {
                        for (count = 0; count < 5; count++) {
                                for(i=0; i<10000; i++);
                                iprintf("In thread %u -- pass %d\r\n", currThread, count);
                        }
                        //release lock
                        lock_release(&UART_LOCK);
                        yield();
                }
        }
}

/*****************************************************************************
// *Insert Comment*
*****************************************************************************/
void OLED_Thread(void)
{
        unsigned count;
        int i;
        while (1)
        {
                volatile long i;
                for(i = 0; i < 100000; i++); // delay
                RIT128x96x4StringDraw("OLED  ", 30, 8, 15);
                for(i = 0; i < 100000; i++); // delay
                RIT128x96x4StringDraw("SCREEN  ", 30, 8, 15);
                for(i = 0; i < 100000; i++); // delay
                RIT128x96x4StringDraw("THREAD", 30, 8, 15);

                yield();
        }
}

/*****************************************************************************
// *Insert Comment*
*****************************************************************************/
void LED_Thread(void)
{
        unsigned count;
        int i;
        while(1)
        {
                LED0_REG ^=1;
                yield();
        }
}
