/*
===============================================================================
 Name        : Subroutines.c
 Author      : Vo Hieu Nghia
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
__attribute__(( naked )) int prt(const char *a)
{
// the pointer to a character array is passed to the
// subroutine in R0
// remember to follow AAPCS:
// push any register above r3 at the beginning and pop same them at the end
// note: lr needs to be pushed and popped so that bx lr returns to right address
asm volatile
(
		"push {lr, r4, r5, r6} \n"

		"mov r4,r0 \n"
		"mov r5,#0 \n"
		"mov r6,#0 \n"

		"mov r0,#0 \n"

		//for loop count
		"count: \n"
		"ldr r0, [r4, r6] \n"
		"cmp r0,#0 \n"
		"beq transfer \n "
		"add r6,r6,#1 \n"
		"b count \n"

		"transfer: \n"
		"mov r5,r6 \n"

		//for loop print
		"loop1: \n"
		"ldr r0, [r4, r5] \n"
		"bl Board_UARTPutChar\n"
		"cmp r5,#0 \n"
		"beq end \n "
		"subs r5,r5,#1 \n"
		"b loop1 \n"

		"end: \n"

		"pop {lr, r4, r5, r6} \n"
		"bx lr \n"
);
Board_UARTPutSTR("Error - escaped outside return.\r\n");
}

// TODO: insert other definitions and declarations here

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // TODO: insert code here
    Board_UARTPutSTR("\r\nHello, World\r\n");
    char test1[] = "Computer Architecture\r\n";
    char test2[] = "Computer Architecture\r\n";
    prt(test1);

    if(strcmp(test1, test2)) {
    	 while(1); // error - string modified
    }
    char test3[] = "Johnny Ca$h:Live @Folsom\r\n";
    char test4[] = "Johnny Ca$h:Live @Folsom\r\n";
    prt(test3);

    if(strcmp(test3, test4)) {
    	while(1); // error - string modified
    }

    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
