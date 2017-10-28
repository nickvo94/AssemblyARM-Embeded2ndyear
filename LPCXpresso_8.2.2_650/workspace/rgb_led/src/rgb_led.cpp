/*
===============================================================================
 Name        : main.c
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
#include <stdint.h>
#include <cr_section_macros.h>
#include <stdlib.h>
#include <time.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#define MAXLEDS 7
static const uint8_t diceport[] = { 0, 1, 0, 0, 0, 0, 1 };
static const uint8_t dicepins[] = { 0, 3, 16, 10, 9, 29, 9 };

void setLED(int idx, bool state) {
	Chip_IOCON_PinMuxSet(LPC_IOCON, diceport[idx], dicepins[idx], IOCON_MODE_INACT | IOCON_DIGMODE_EN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, diceport[idx], dicepins[idx]);
	Chip_GPIO_SetPinState(LPC_GPIO, diceport[idx], dicepins[idx], state);
	return;
}

void setupButton() {
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 0, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 0);

	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 8, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 8);
}

void diceCase (int RandomNum){
	switch (RandomNum) {
				case 0:
					setLED(0, false);
					setLED(4, false);
					setLED(6, false);		//PD1
					setLED(2, false);		//PD5
					setLED(1, false);		//PD6
					setLED(5, false);		//PD2
					setLED(3, false);
					break;
				case 1:
					setLED(3, true);		//PD4
					break;
				case 2:
					setLED(0, true);		//PD7
					setLED(4, true);		//PD3
					break;
				case 3:
					setLED(0, true);
					setLED(4, true);
					setLED(3, true);
					break;
				case 4:
					setLED(0, true);
					setLED(4, true);
					setLED(6, true);		//PD1
					setLED(2, true);		//PD5
					break;
				case 5:
					setLED(0, true);
					setLED(4, true);
					setLED(6, true);		//PD1
					setLED(2, true);
					setLED(3, true);
					break;
				case 6:
					setLED(0, true);
					setLED(4, true);
					setLED(6, true);		//PD1
					setLED(2, true);		//PD5
					setLED(1, true);		//PD6
					setLED(5, true);		//PD2
					break;
				case 7:
					setLED(0, true);
					setLED(4, true);
					setLED(6, true);		//PD1
					setLED(2, true);		//PD5
					setLED(1, true);		//PD6
					setLED(5, true);		//PD2
					setLED(3, true);
					break;
	}
}


int main(void) {
//setLED(5,true);

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
Board_Init();
    // Set the LED to the state of "On
    //Board_LED_Set(0, false);
#endif
#endif
	//while(true){
    // TODO: insert code here

	bool 	buttonPress1;
	bool 	buttonPress2;
	int 	RandomNum;
	int		but1rel = 0;
	int		but2rel = 0;


	// Set up button pin modes for input
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 0, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 8, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 8);

	srand (time(NULL));

	while(true) {
		buttonPress1 = Chip_GPIO_GetPinState(LPC_GPIO, 1, 0);

		if ( buttonPress1 ) {
			while (Chip_GPIO_GetPinState(LPC_GPIO, 1, 0)) {
				diceCase(7);
				but1rel = 1;
				}
		}
		if (buttonPress1 == false && but1rel == 1){
			diceCase(0);
			but1rel = 0;
		}

		buttonPress2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 8);	//setLED(4, buttonPress2);

		if (buttonPress2){
			diceCase(0);
			RandomNum = rand()%6+1 ;
			but2rel = 1;
		}

		if (buttonPress2 == false && but2rel == 1){
			diceCase(RandomNum);
			but2rel = 0;
		}
			// Clear all LEDs


			//buttonPress2 = false;

	}

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    //while(1) {
        //i++ ;
    //}
    return 0 ;
};


