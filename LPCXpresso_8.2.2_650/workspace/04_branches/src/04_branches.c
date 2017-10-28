/*
===============================================================================
 Name        : 04_branches.c
 Author      : $(author)
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

// TODO: insert other definitions and declarations here

__attribute__(( naked )) int abs_sum(int a, int b)
{
	// the values passed to your program are in registers
	// R0 = a, R1 = b
	// write your code between push and pop instructions
	// make sure return value is in R0 after calculation
	asm volatile
	(
		"push {r4, r5, r6, r7} \n"
		"loc1: mov r4, #0 \n"
		// execute compare instruction to set flags before conditional branch

		"mov r4, r0 \n"
		"cmp r4, #0 \n"
		"bpl L1 \n"
		"mvns r4, r0 \n"
		"adds r4, r4, #1 \n "

		"L1: mov r5, r1 \n"
		"cmp r1, #0 \n"
		"bpl L2 \n"
		"mvns r5, r1 \n"
		"adds r5, r5, #1 \n "

		"L2 : adds r6, r4, r5 \n"
		"mov r0, r6 \n"

		"beq loc1 \n"
		"pop {r4, r5, r6, r7} \n"
		"bx lr \n"
	);
}

void fail() {
	printf("Failed\n");
}


int main(void) {

/*#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif*/

    // TODO: insert code here
    if(abs_sum(-9, 7) != 16) fail();
    if(abs_sum(-19, -1) != 20) fail();
    if(abs_sum(22, 5) != 27) fail();
    if(abs_sum(8, 18) != 26) fail();
    if(abs_sum(0, 7) != 7) fail();
    if(abs_sum(-1, 0) != 1) fail();

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
