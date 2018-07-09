# EGR424Project3-Kernel
You will be developing a pre-emptive multi-threaded kernel on the LM3S6965 microcontroller. The end result of your efforts will be a kernel that provides an abstracted view of the hardware, such that the application developer can be concerned only with writing independent threads/processes to perform system functions. Thread creation, thread switching, scheduling, etc. will all be handled by your kernel. 


# Project Requirements

• You are to implement and document a working kernel for the LM3S6965.

• The kernel is to support multiple concurrent threads of execution (a.k.a. tasks).

• The kernel is to be pre-emptive such that each thread may have its execution suspended at any time.

• The kernel is to pre-empt each thread at 1ms intervals using the SysTick timer peripheral.

• A thread may give up its 1ms “time slice” at any time by calling a yield() function.

• All threads are to run in the unprivileged thread mode of the Cortex-M3 processor and to use the process
stack. The kernel must run in privileged handler mode and use the main stack. Every thread is to have its
own separate stack.

• All communication between threads and the kernel is to be done through C API calls (not global
variables). Communications between threads may use global variables/memory (e.g., circular buffers).

• All interrupt service routines and hardware interactions are considered to be part of the kernel. Threads
are not allowed to enable or disable interrupts and must interact with the hardware through C API
function calls only.

• There are to be no busy-wait loops anywhere in the kernel or threads, except perhaps for 1 “idle thread”
that does nothing and runs only when there is no work to be done in either user threads or the kernel.

  • ANY for-loop or while-loop (or any kind of loop) in the kernel is suspicious and must be
justified.

    •  A loop in the kernel that iterates over active threads looking for the next thread to run is OK. 

    • A loop in the kernel that samples a pushbutton 5 times, with a delay in between each sample for the purposes of debouncing, is NOT OK. Debouncing can be done in a user thread or intelligently in the kernel (without loops or delays).

    • A loop in the kernel that computes the average power of the last ‘N’ A/D samples is not OK. This loop belongs in a user thread.

• ISR’s must be FAST and SHORT: handle the hardware event, move some data, set a flag, etc.
then exit ASAP. There should be NO significant work or computation in the kernel. 

# EXTRA CREDIT: 

Instead of an idle thread, put the processor in a low-power sleep mode and have it awaken
in response to an interrupt indicating that a thread is now ready to do some useful work. This can be
incorporated into the idle thread if it makes it easier: 
