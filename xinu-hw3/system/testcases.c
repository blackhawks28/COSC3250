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

    unsigned char x;

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
            kungetc('c');
            kungetc('d');
            kungetc('e');
            kungetc('f');
            kungetc('g');
            kungetc('h');
            kungetc('i');
            kungetc('j');
            kungetc('k');
            break;
            // TODO: Test your operating system!

        default:
            kprintf("Hello Xinu World!\r\n");
    }


    kprintf("\r\n===TEST END===\r\n");
    return;

}
