#define _GNU_SOURCE
#include "simple_mutex.h"
#include <linux/futex.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdint.h>

void mutex_lock(int *mutex)
{  
	int v;
	
	// Obtain the value of the first bit of mutex
	int prev = __atomic_fetch_or(mutex, 1<<31, __ATOMIC_RELAXED);
    
    // If prev is not equivalent to 0x80000000 then thread obtained the
    // lock because bit 31 was zero. We can return because the work is done.
    // In other words, the lock was free.
    if ((prev & (1<<31)) == 0) return;
	
	// Otherwise we must add 1 because we now have a waiter
    __atomic_add_fetch (mutex, 1, __ATOMIC_RELAXED);
    
    // While waiting thread has not obtained the lock...
    while (1) {
    
    	// Obtain the value of the mutex
    	int new_prev = __atomic_fetch_or (mutex, 1<<31, __ATOMIC_RELAXED);
    	
    	// If this is true, we know that the lock is free. We can then
    	// decrement the mutex because a waiting thread obtained the lock
    	// so it is no longer waiting. Job is done
		if ((new_prev & (1<<31)) == 0) {
 			__atomic_sub_fetch (mutex, 1, __ATOMIC_RELAXED);
 			return;
 		}
 	
		// Check if lock is taken. If it is not taken, while loop body reexecutes
		// at this point the thread should be able to obtain the lock
        v = *mutex;
        if (v >= 0)
            continue;
            
        // If the lock is still taken, the calling process is essentially
        // put to sleep until a change in the value of the mutex is encountered
        syscall (SYS_futex, mutex, FUTEX_WAIT, v);
    }
}

void mutex_unlock(int *mutex)
{  

	/* Adding 0x80000000 to counter results in 0 if and
       only if there are not other interested threads */
    if ((__atomic_add_fetch (mutex, 1<<31, __ATOMIC_RELAXED)) == 0)
		return;
		
    /* There are other threads waiting for this mutex,
     wake one of them up. */
    syscall (SYS_futex, mutex, FUTEX_WAKE);
}
