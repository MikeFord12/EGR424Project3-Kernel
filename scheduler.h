#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

extern unsigned threadlock;
extern void lock_init(unsigned *lock);
extern unsigned lock_acquire(unsigned *lock);
extern void lock_release(unsigned *lock);

extern unsigned currThread;
extern void yield(void);

#endif // _SCHEDULER_H_
