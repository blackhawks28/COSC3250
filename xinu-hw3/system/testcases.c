/**
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by:
 *
 * and
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
        case 'a':
            kprintf("AYE PAPI MY LITTLE ASSHOLE IS SO FUCKING BIG RIGHT NOW! LOS MEMOS\n");
            break;
        case 'b':
            kprintf("LOS MEMOS IM GETTING SMASHED RIGHT NOW!");
            break;
        case 'c':
            //kungetc(x);
            //kprintf("%c",'h');
            //kprintf("%d",'H');
            kungetc('a');
            kungetc('b');
            kungetc('t');

            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            kprintf("%c",kgetc());


            break;
        case 'd':

            kungetc('N');
            kungetc('O');
            kungetc('D');
            kungetc('E');
            kungetc('P');
            kungetc('P');
            kungetc('A');
            kungetc('L');
            kungetc('C');
            kungetc('T');
            kungetc('E');
            kungetc('G');

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
            kprintf("%c",kgetc());
            kprintf("%c",kgetc());
            // TODO: Test your operating system!
            break;
        default:
            kprintf("Hello Xinu World!\r\n");
    }


    kprintf("\r\n===TEST END===\r\n");
    return;
}
