/*
===============================================================================
 Name        : Addressing_modes.c
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
__attribute__(( naked )) void asm_test(int *a, int *b, int *c, int *d)
{

asm volatile
(
		"push {r4, r5, r6, r7} \n"
		"ldr r4, [r0] \n"
		"ldr r5, [r1] \n"			//r5 = M2
		"adds r6, r4, r5 \n"		//r6 = M1+M2
		"ldr r4, [r3] \n"			//r4 <- M4
		"muls r4, r5, r4 \n"		//r4 = M2*M4
		"ldr r5, [r2] \n"			//r5 <- M3
		"adds r7, r4, r5 \n"		//r7 = M3+(M2*M4)
		"muls r7, r6, r7 \n"		//r7 = (M1+M2)*(M3+(M2*M4))
		"str r7,[r1] \n"
		"pop {r4, r5, r6, r7} \n"
		//"bx lr \n"
);
}

void fail() {
	//printf("Failed\n"); // set a break point here
}

void tst(void) {
    short a[7] = { 0 };
    short *v = a;
    short k = 0;
    short temp = 0;
    temp = v[k];
    v[k] = v[k+1];
    v[k+1] = temp;
}

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
    tst();

    int m1;
	int m2;
	int m3;
	int m4;
	m1 = 1; m2 = 2; m3 = 3; m4 = 4;
	asm_test(&m1, &m2, &m3, &m4);
	if(m2 != 33) { fail(); }

	// Force the counter to be placed into memory
    volatile static int i = 0;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0;
}
