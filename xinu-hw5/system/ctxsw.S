/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */
/**
* COSC 3250 - Project 5
* Switches the process that is currently being ran and starts a new one then swaps them back after the new one runs.
* @authors [Chris Piszczek]
* @authors [Dennis Burmeister]
* Instructor [Dr. Rubya]
* TA-BOT:MAILTO [christian.piszczek@marquette.edu]
* TA-BOT:MAILTO [dennis.burmeister@marquette.edu]
*/
#include <arm.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldregs, &newregs)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldreg address of outgoing stack save area -> r0 when called
 * @param  &newreg address of incoming stack save area -> r1 when called
 * @return special case -- see above

//source for part of code and conceptual understanding https://www.youtube.com/watch?v=DKmBRl8j3Ak&ab_channel=Computerphile
 oldregs is teh current info, will be r0 when called
 */
ctxsw:
	.func ctxsw

    str r0, [r0, #PREG_R4 * 0]
    str r1, [r0, #PREG_R4 * 1]
    str r2, [r0, #PREG_R4 * 2]
    str r3, [r0, #PREG_R4 * 3]
    str r4, [r0, #PREG_R4 * 4]
    str r5, [r0, #PREG_R4 * 5]
    str r6, [r0, #PREG_R4 * 6]
    str r7, [r0, #PREG_R4 * 7]
    str r8, [r0, #PREG_R4 * 8]
    str r9, [r0, #PREG_R4 * 9]
    str r10, [r0, #PREG_R4 * 10]
    str r11, [r0, #PREG_R4 * 11]
    str r13, [r0, #PREG_R4 * 13]
    str r14, [r0, #PREG_R4 * 14]

    mov r12, r1                 //Save copy of oldregs
    ldr r0, [r1]                //Load new regs

    // TODO: Restore callee-save ("non-volatile") registers.

     ldr r0,[r12, #PREG_R4 * 0]
     ldr r1, [r12, #PREG_R4 * 1]
     ldr r2, [r12, #PREG_R4 * 2]
     ldr r3, [r12, #PREG_R4 * 3]
     ldr r4, [r12, #PREG_R4 * 4]
     ldr r5, [r12, #PREG_R4 * 5]
     ldr r6, [r12, #PREG_R4 * 6]
     ldr r7, [r12, #PREG_R4 * 7]
     ldr r8, [r12, #PREG_R4 * 8]
     ldr r9, [r12, #PREG_R4 * 9]
     ldr r10, [r12, #PREG_R4 * 10]
     ldr r11, [r12, #PREG_R4 * 11]
     ldr r13, [r12, #PREG_R4 * 13]
     ldr r14, [r12, #PREG_R4 * 14]

    // TODO: Jump to next function.

    ldr r15, [r12, #PREG_R4 * 15]
	// TODO: Save callee-save ("non-volatile") registers. USE VOLATILE REGISTERS NOT NON-VOLATILE REGISTERS
	// TODO: Restore callee-save ("non-volatile") registers. USE VOLATILE REGISTERS NOT NON-VOLATILE REGISTERS
	// TODO: Jump to next function.

    //use a bunch of ldr and str to swap process; save r0 in another register(in ip, instruction pointer), and then load r1
	.end ctxsw
//ldr str mov,
//figure out registers 5-15 and what they do