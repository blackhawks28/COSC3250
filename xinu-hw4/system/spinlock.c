/**
 * @file spinlock.c
 * @provides lock_create, lock_free, lock_acquire, lock_release.
 *
 * Embedded Xinu, Copyright (C) 2008, 2018. All rights reserved.
 */
/**
* COSC 3250 - Project 4
* Creates, frees, acquires, and releases the locks.
* @authors [Chris Piszczek]
 * @authors [Dennis Burmeister]
* Instructor [Dr. Rubya]
* TA-BOT:MAILTO [christian.piszczek@marquette.edu]
* TA-BOT:MAILTO [dennis.burmeister@marquette.edu]
*/

#include <xinu.h>

struct lockent locktab[NLOCK];  /* spinlock table */

/* Assembly helper functions in spinlock_util.S */
extern void _lock_acquire(unsigned int *);
extern void _lock_release(unsigned int *);
extern int _atomic_lock_check(unsigned int *);

static spinlock_t lock_alloc(void);

/**
 * Create and initialize a spinlock, returning its ID.
 * @return new lock ID on success, SYSERR on failure
 */
spinlock_t lock_create()
{
    spinlock_t lock;

    lock = lock_alloc();
    if (SYSERR == lock)
    {
        return SYSERR;
    }

    locktab[lock].lock = SPINLOCK_UNLOCKED;
    return lock;
}

/**
 * Deallocate a lock.
 * Reset the lock entry, and deallocate 
 * the entry from the global lock table.
 * NOTE: This function can potentially 
 * 		 not be thread safe. Undefined behavior
 * 		 can occur if more than one thread
 * 		 attempts to free a lock.
 *       
 * @param lock	target lock
 * @return OK on success, SYSERR on failure
 */
syscall lock_free(spinlock_t lock)
{
    if (isbadlock(lock))
        return SYSERR;

    locktab[lock].state = SPINLOCK_FREE;

    return OK;
}

/**
 * Acquire a lock.
 * @param lock	target lock
 * @return OK on success, SYSERR on failure
 */
syscall lock_acquire(spinlock_t lock) {
    // TODO: First, check if lock is a valid lock.
    //       Next, call _lock_acquire assembly subroutine
    //       and properly set "core" field of lockent struct
    int x = isbadlock(lock);

    if(x == 1){
        return SYSERR;
    }
        _lock_acquire(&(locktab[lock].lock));
        locktab[lock].core = getcpuid();
        return OK;
}
/**
 * release a lock
 * @param lock	target lock
 * @return OK on success, SYSERR on failure
 */
syscall lock_release(spinlock_t lock) {
    // TODO: Check if lock is a valid lock.
    //       Call _lock_release assembly subroutine and
    //       reset "core" field of lockent struct

    int x = isbadlock(lock);

    if(x == 1){
        return SYSERR;
    }
    else {
        _lock_release(&(locktab[lock].lock));
        locktab[lock].core = -1;
        return OK;
    }
}


/**
 * Allocate an unused lock and return its ID.
 * Scan the global lock table for a free entry, mark the entry
 * used, and return the new lock.
 * @return available lock ID on success, SYSERR on failure
 */
static spinlock_t lock_alloc(void)
{
    int i;
    static spinlock_t nextlock = 0;

    /* check all entries */
    for (i = 0; i < NLOCK; i++)
    {
        nextlock = (nextlock + 1) % NLOCK;
        if (0 ==_atomic_lock_check((unsigned int *)&(locktab[nextlock].state)))
        {
            return nextlock;
        }
    }
    return SYSERR;
};
