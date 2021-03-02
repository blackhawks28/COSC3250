/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/**
* COSC 3250 - Project 5
* Initilizes and creates a new process and sends the args into the stack for ctxsw to call to.
* @authors [Chris Piszczek]
* @authors [Dennis Burmeister]
* Instructor [Dr. Rubya]
* TA-BOT:MAILTO [christian.piszczek@marquette.edu]
* TA-BOT:MAILTO [dennis.burmeister@marquette.edu]
*/

/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <arm.h>
#include <xinu.h>

#define MAX 15

/* The standard ARM calling convention passes first four arguments in r0-r3; the
 * rest spill onto the stack.  */
#define MAX_ARGS 4


/* Assembly routine for atomic operations */
extern int _atomic_increment_post(int *);
extern int _atomic_increment_limit(int *, int);

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, char *name, ulong nargs, ...)//nargs number of parameters
{
    ulong *remember;
    int overflowArgs = 0;
    int normargs = 0;
	ulong *saddr;               /* stack address                */ // ulong is only positive values, 4 bites long
	ulong pid;                  /* stores new process id        */ //each process, pid is saved
	pcb *ppcb;                  /* pointer to proc control blk  */
	ulong i;
	va_list ap;                 /* points to list of var args   */
	ulong pads = 0;             /* padding entries in record.   */
	void INITRET(void);

	if (ssize < MINSTK)
		ssize = MINSTK;
	ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;//OxFFFFFFFC == 1 GB of RAM
	/* round up to even boundary    */
	saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
	pid = newpid();
	/* a little error checking      */
	if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
	{
		return SYSERR;
	}

	_atomic_increment_post(&numproc);

	ppcb = &proctab[pid];// pointer from control bulk = memory address of new process id
	/* setup PCB entry for new proc */
	ppcb->state = PRSUSP;//PRSUSP suspended state since it isnt ready yet

	// TODO: Setup PCB entry for new process.

    //setup ^ppcb entry for this particular process;need to setup the stack; stack address saddr, to setup stack use *stkbase in proc.h
    //ppcb->stkbase =?
    ppcb->stkbase = (char *)saddr;
    ppcb->stklen = ssize;
    //setup core-affinity, -1 for initial
    ppcb->core_affinity = -1;
    //setup process name
//    for(int i = 0; i <strlen(name);i++){
//        ppcb->name[i] = name[i];
//    }
    strcpy(ppcb->name,name);

    /* Initialize stack with accounting block. */
	*saddr = STACKMAGIC;
	*--saddr = pid;//putting pid in stack
	*--saddr = ppcb->stklen;//putting length in stack for the process in the memory
	*--saddr = (ulong)ppcb->stkbase;

	//kprintf("%X\r\n",saddr);

	/* Handle variable number of arguments passed to starting function   */
	if (nargs)
	{
		pads = ((nargs - 1) / 4) * 4;
	}
	/* If more than 4 args, pad record size to multiple of native memory */
	/*  transfer size.  Reserve space for extra args                     */
	for (i = 0; i < pads; i++)
	{
		*--saddr = 0;//going down in the stack to add info; look up how a stack works to understand why we are decrementing
	}


	if(nargs > MAX_ARGS){
	    overflowArgs = nargs - MAX_ARGS;
	    normargs = MAX_ARGS;
	}
	else{
	    overflowArgs = 0;
	    normargs = nargs;
	}


    for(int i = nargs; i > 0; i--){
        *--saddr = 0;
    }
    remember = saddr;
    for(int i = MAX; i > 0; i--){
        *--saddr = 0;
    }
	va_start(ap,nargs);
	for(int  i = 0; i < normargs; i++){
	    ppcb->regs[i] = va_arg(ap,ulong);
	    saddr++;
	}
	saddr = saddr - 1;

    //r0-r3 gets put using regs
    //pc,lr,sp gets put in using regs
   // you only use *--saddr when there are more than 4 total arguments. ==> *--saddr = va_arg()

    for(int i = overflowArgs; i < MAX-3; i++){
        *++saddr = va_arg(ap,ulong);
        //kprintf("%X\n",remember);
    }
	va_end(ap);
	// TODO: Initialize process context.
	//
	// TODO:  Place arguments into activation record.               use the macros for the variable number of parameters
	//        See K&R 7.3 for example using va_start, va_arg and
	//        va_end macros for variable argument functions.

	//HAVE TO CHECK IF THERE ARE MORE THAN 4 ARGUMENTS AND HAVE TO STORE THEM TO USE
	//kprintf("%d",ppcb->regs);
    ppcb->regs[(MAX)] = (ulong)funcaddr;//was 1
    ppcb->regs[(MAX)-1] = (ulong)userret;//was 2
    ppcb->regs[(MAX)-2] = (ulong)saddr;


    //kprintf("0x%08X\r\n",ppcb->regs[15]);
    //kprintf("0x%08X\r\n",ppcb->regs[14]);
    //kprintf("0x%08X\r\n",ppcb->regs[13]);//0x3EFFFFD4


	return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
	pid_typ pid;                /* process id to return     */
	static pid_typ nextpid = 0;

	for (pid = 0; pid < NPROC; pid++)
	{                           /* check all NPROC slots    */
		//        nextpid = (nextpid + 1) % NPROC;
		_atomic_increment_limit(&nextpid, NPROC);
		if (PRFREE == proctab[nextpid].state)
		{
			return nextpid;
		}
	}
	return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
	uint cpuid = getcpuid();
	kill(currpid[cpuid]);
}
