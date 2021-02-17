/**
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by: Chris Piszczek & Dennis Burmeister
 *
 * Instructor[Dr. Rubya]
 * TA-BOT:MAILTO[christian.piszczek@marquette.edu]
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

devcall putc(int dev, char c)
{
    return 0;
}


/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{

    int c;

    kprintf("===TEST BEGIN===\r\n");

    c = kgetc();

    switch (c)
    {
        case 'a'://testing kprintf and kputc work properly
            kprintf("Hello this is the result to selecting test case a.\r\n");
            break;

        case 'b'://testing kprintf and kputc work properly
            kprintf("Hello this is the result to selecting test case b.\r\n");
            break;

        case 'c'://testing that a simple input and outputs work properly
            kungetc('a');
            kungetc('b');
            kungetc('c');

            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            break;

        case 'd'://Testing that a longer input and output work properly

            kungetc('D');
            kungetc('L');
            kungetc('R');
            kungetc('O');
            kungetc('W');
            kungetc('O');
            kungetc('L');
            kungetc('L');
            kungetc('E');
            kungetc('H');


            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            kprintf("%c",kgetc());


            break;

        case 'e'://testing if there are more than 10 inputs that the kungetc returns SYSERR

            kungetc('C');
            kungetc('S');
            kungetc('I');
            kungetc('E');
            kungetc('M');
            kungetc('A');
            kungetc('N');
            kungetc('Y');
            kungetc('M');
            kungetc('I');
            int c =  kungetc('H');
            if(c == SYSERR){
                kprintf("Error, to many characters\r\n");
                break;
            }

            break;

        case 'f'://testing kcheckc to make sure it returns false when nothing is in the array

            if(kcheckc() == 0){
                kprintf("There is nothing in the array\r\n");
            }

            break;

        case 'g'://testing the kungetc,kcheckc,kgetc, kprintf, and kputc all in one test. Making sure that an input works and that
            //kcheckc returns true for whether there is an element in the array. Then using kprintf and kputc to output statement. Next, removing
            //the element with kgetc, and testing kcheckc again to return false.
            kungetc('1');
            if(kcheckc() == 1){
                kprintf("There is an element in the array.\r\n");
            }
            kprintf("%c\r\n",kgetc());
            if(kcheckc() == 0){
                kprintf("There is no longer an element in the array.\r\n");
            }

            break;

        default:
            kprintf("No characters were entered.\n");
    }

    kprintf("\r\n===TEST END===\r\n");
    return;

}
