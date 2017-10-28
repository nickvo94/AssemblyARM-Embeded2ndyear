/*
===============================================================================
 Name        : Subroutines.c
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

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();

#endif
#endif

    // TODO: insert code here
    int x;
    char array[50] = "Nothing";
    for (int n=0; n<50; n++){
    	x = array[n];
    	Board_UARTPutChar(x);
    	if (x == 0){n = 60;}
    }

    return 0 ;
}
