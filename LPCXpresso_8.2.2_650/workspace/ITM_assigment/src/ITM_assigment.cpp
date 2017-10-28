/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#define TICKRATE_HZ1 (1000)

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif
#include <iostream>
#include <cstdio>
#include <stdio.h>

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "ITM_write.h"

// TODO: insert other definitions and declarations here

static volatile int counter;
#ifdef __cplusplus
extern "C" {
#endif
/**
* @brief Handle interrupt from SysTick timer
* @return Nothing
*/

void SysTick_Handler(void)
{
	if(counter > 0) counter--;
}
#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
	counter = ms;
	while(counter > 0) {
		__WFI();
	}
}

class LedMenuSys {
	int menuItem;
	bool led[3];
public:
	LedMenuSys(void);
	void update(void);
	void move(int);
	void toggleLed(void);
private:
};

LedMenuSys :: LedMenuSys(void) {
	for (int i=0; i<3; i++) {
		led[i] = false;
	}
	menuItem = 0;
	Board_LED_Set(0, false);
	Board_LED_Set(1, false);
	Board_LED_Set(2, false);
	update();
}

void LedMenuSys :: update(void) {
	char text[40];
	for (int i=0; i < 3; i++) {
		switch(i) {
		case 0:
			ITM_write(" Red   ");
			break;
		case 1:
			ITM_write(" Green ");
			break;
		case 2:
			ITM_write(" Blue  ");
			break;
		}
		if ( led[i] == true ) {
			ITM_write("on ");
		} else {
			ITM_write("off");
		}
		if ( i == menuItem ) { ITM_write( " <--"); }
		ITM_write(" \n");
	}
}

void LedMenuSys :: move(int amount) {
	menuItem += amount;
	if (menuItem > 2) { menuItem = 2; }
	if (menuItem < 0) { menuItem = 0; }
	update();
}

void LedMenuSys :: toggleLed(void) {
	led[menuItem] = !led[menuItem];
	Board_LED_Set(menuItem, led[menuItem]);
	update();
}

class DigitalIoPin {
	int port;
	int pin;
	int IdNum;
	bool hold;
	//int counterNum = 0;
	public:
		DigitalIoPin (int, int, bool, bool, bool, int);
		//virtual ~DigitalIoPin();
		bool read();
		bool write();
		int count();
		//int menu();
	private:
};

DigitalIoPin :: DigitalIoPin(int setPin, int setPort, bool input, bool pullup, bool invert, int Id) {
	//ITM_write("IoPin object added.\n");
	port = setPort;
	pin = setPin;
	IdNum = Id;
	//int ModeSet = IOCON_MODE_PULLUP;
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	if (input) {
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	} else {
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
	}
}

bool DigitalIoPin :: read(){

	return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);

}

bool DigitalIoPin :: write(){
	return false;
}

int DigitalIoPin :: count (){
	int timeInMs = 0;
	while (Chip_GPIO_GetPinState(LPC_GPIO, port, pin)) {
				Sleep(10);
				timeInMs++;
	}
	return timeInMs * 10;
}

int main(void) {


	uint32_t sysTickRate;

	//SystemCoreClockUpdate();

	/* The sysTick counter only has 24 bits of precision, so it will
	   overflow quickly with a fast core clock. You can alter the
	   sysTick divider to generate slower sysTick clock rates. */
	/* A SysTick divider is present that scales the sysTick rate down
	   from the core clock. Using the SystemCoreClock variable as a
	   rate reference for the SysTick_Config() function won't work,
	   so get the sysTick rate by calling Chip_Clock_GetSysTickClockRate() */
	/* Enable and setup SysTick Timer at a periodic rate */

	#if defined (__USE_LPCOPEN)
	#if !defined(NO_BOARD_LIB)
		Board_Init();
	#endif
	#endif
	SystemCoreClockUpdate();

	Chip_Clock_SetSysTickClockDiv(1);
	sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / TICKRATE_HZ1);

    ITM_init();

    DigitalIoPin button1 (17, 0, true, true, false, 0);
	DigitalIoPin button2 (11, 1, true, true, false, 0);
	DigitalIoPin button3 (9, 1, true, true, false, 0);

	LedMenuSys menu;

	Sleep(1000);

	while(1) {

		if (button2.read()) {
			button2.count();
			menu.toggleLed();
		}
		if (button3.read()) {
			button3.count();
			menu.move(1);
		}
		if (button1.read()) {
			button1.count();
			menu.move(-1);
		}
    }
    return 0 ;
}
