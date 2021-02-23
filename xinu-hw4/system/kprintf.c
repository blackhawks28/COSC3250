/**
 * @file kprintf.c
 * COSC 3250 - Project 3
 *
 *@author[Chris Piszczek & Dennis Burmeister]
 *Instructor[Dr. Rubya]
 *TA-BOT:MAILTO[christian.piszczek@marquette.edu]
 *
 */

/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

#define UNGETMAX 10             /* Can un-get at most 10 characters. */


static unsigned char ungetArray[UNGETMAX] = {NULL};

/**
 * Synchronously read a character from a UART.  This blocks until a character is
 * available.  The interrupt handler is not used.
 *
 * @return
 *      The character read from the UART as an <code>unsigned char</code> cast
 *      to an <code>int</code>.
 */
syscall kgetc(void)
{
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    // TODO: First, check the unget buffer for a character. (kcheckc)
    //       Otherwise, check UART flags register, and
    //       once the receiver is not empty, get character c.
    int ava = kcheckc();
	
	while(ava == 0){//waiting for input once k puts c in kputsc()
		ava = kcheckc();
	}
	
	int i = 9;//9
	while(i >= 0){
		if(ungetArray[i] != NULL){
			int c = (int)ungetArray[i];
			ungetArray[i] = NULL;
			
			return c;
		}
		
		i--;
	}
    
	int x = (int)regptr->dr;
		
	return x;
    

    return SYSERR;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
    volatile struct pl011_uart_csreg *regptr;
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    // TODO: Check the unget buffer and the UART for characters.
	int i = 0;
	while(i < 10){
		if(ungetArray[i] != NULL){
			return TRUE;
		}
		i++;
	}
	if(regptr->fr & (PL011_FR_RXFE)){
		return FALSE;
	}
	else{
		return TRUE;
	}
	
    return SYSERR;
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
    // TODO: Check for room in unget buffer, put the character in or discard.
    int i = 0;
	
	while(i < 10){

	    if(ungetArray[i] == NULL){
	
	        ungetArray[i] = c;

	        i++;
			
			return c;   
	    }
		
		i++;
	}
	
    return SYSERR;
}


/**
 * Synchronously write a character to a UART.  This blocks until the character
 * has been written to the hardware.  The interrupt handler is not used.
 *
 * @param c
 *      The character to write.
 *
 * @return
 *      The character written to the UART as an <code>unsigned char</code> cast
 *      to an <code>int</code>.
 */
syscall kputc(uchar c)
{
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    // TODO: Check UART flags register.
    //       Once the Transmitter FIFO is not full, send character c.
	

	while(regptr->fr & (PL011_FR_TXFF));
	regptr->dr = c;
	return (int)c;
			

    return SYSERR;
}

/**
 * kernel printf: formatted, synchronous output to SERIAL0.
 *
 * @param format
 *      The format string.  Not all standard format specifiers are supported by
 *      this implementation.  See _doprnt() for a description of supported
 *      conversion specifications.
 * @param ...
 *      Arguments matching those in the format string.
 *
 * @return
 *      The number of characters written.
 */
syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    lock_acquire((int)serial_lock);

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(int, int))kputc, 0);
    va_end(ap);
	
	lock_release((int)serial_lock);
	
    return retval;
}