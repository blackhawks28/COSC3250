/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

extern void ctxsw(void *, void *);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
syscall resched(void)
{
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */

	uint cpuid = getcpuid();

    oldproc = &proctab[currpid[cpuid]];

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid[cpuid], readylist[cpuid]);
    }

    /* remove first process in ready queue */
    currpid[cpuid] = dequeue(readylist[cpuid]);
    newproc = &proctab[currpid[cpuid]];
    newproc->state = PRCURR;    /* mark it currently running    */


    //kprintf("%08x\n",&oldproc->regs);
    //kprintf("%08x\n",&newproc->regs);
    ctxsw(&oldproc->regs, &newproc->regs);
    //kprintf("penis%08x\r\n",&oldproc->regs);
    //kprintf("penis1%08x\r\n",&newproc->regs);
    /* The OLD process returns here when resumed. */
    return OK;
}
