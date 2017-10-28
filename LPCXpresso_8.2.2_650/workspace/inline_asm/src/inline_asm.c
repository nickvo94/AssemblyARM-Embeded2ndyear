/*
===============================================================================
 Name        : inline_asm.c
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
	printf("Failed\n"); // set a break point here
}
void success() {
	printf("Success\n");
}

int main(void) {
	printf("Start test\n");
	int m1;
	int m2;
	int m3;
	int m4;
	m1 = 1; m2 = 2; m3 = 3; m4 = 4;
	asm_test(&m1, &m2, &m3, &m4);
	if(m2 != 33) { fail(); } else { success(); }
	m1 = 8; m2 = 5; m3 = 6; m4 = 21;
	asm_test(&m1, &m2, &m3, &m4);
	if(m2 != 1443) fail();
	m1 = 3; m2 = 4; m3 = 5; m4 = 1;
	asm_test(&m1, &m2, &m3, &m4);
	if(m2 != 63) fail();
	m1 = 3; m2 = 5; m3 = 7; m4 = 8;
	asm_test(&m1, &m2, &m3, &m4);
	if(m2 != 376) fail();
	m1 = 33; m2 = 22; m3 = 11; m4 = 0;
	asm_test(&m1, &m2, &m3, &m4);
	if(m2 != 605) fail();
	m1 = 42; m2 = 55; m3 = 12; m4 = 1;
	asm_test(&m1, &m2, &m3, &m4);
	if(m2 != 6499) fail();
}
