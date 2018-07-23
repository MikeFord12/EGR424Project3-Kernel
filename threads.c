#include <stdio.h>
#include "scheduler.h"

// These are the user-space threads. Note that they are completely oblivious
// to the technical concerns of the scheduler. The only interface to the
// scheduler is the single function yield() and the global variable
// currThread which indicates the number of the thread currently
// running.

void thread1(void)
{
        unsigned count;

        for (count = 0; count < 10; count++) {
                iprintf("In thread %u -- pass %d\r\n", currThread, count);
                yield();
        }
}

void thread2(void)
{
        unsigned count;

        for (count=0; count < 5; count++) {
                iprintf("In thread %u -- pass %d\r\n", currThread, count);
                yield();
        }
}

void thread3(void)
{
        unsigned count;

        for (count=0; count < 5; count++) {
                iprintf("In thread %u -- pass %d\r\n", currThread, count);
                yield();
        }
}
