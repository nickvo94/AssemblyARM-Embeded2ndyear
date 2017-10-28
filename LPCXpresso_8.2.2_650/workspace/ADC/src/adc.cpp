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
#include "DigitalIOPin.h"

#include "ITM_write.h"

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
#define TICKRATE_HZ (1000)	/* 100 ticks per second */
static volatile uint32_t ticks;
static volatile uint32_t counter;
static volatile uint32_t counter2;
static volatile uint32_t ledRate;
static volatile uint32_t toggle;
static volatile uint32_t a0_1;
static volatile uint32_t a0_2;
static volatile uint32_t a0_3;
static volatile uint32_t a10_1;
static volatile uint32_t a10_2;
static volatile uint32_t a10_3;

extern "C" {

void SysTick_Handler()
{

	static uint32_t count;
	/* Every 1/2 second */
	count++;
	static bool blink;
	static int count2;
	if(count2 < 100){count2++;}else if(count2 == 100){Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);count2 = 0;}

	if (count >= TICKRATE_HZ / ledRate) {
		Board_LED_Set(0, false);
		Board_LED_Set(1, false);
		Board_LED_Set(2, false);
		Board_LED_Set(toggle, blink);
		blink = !blink;
		count = 0;
		//Board_LED_Toggle(1);
	}

	ticks++;

	if(counter > 0) counter--;
}

void ADC0A_IRQHandler(void)
{
	uint32_t pending;
	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags(LPC_ADC0);
	/* Sequence A completion interrupt */
	if (pending & ADC_FLAGS_SEQA_INT_MASK) {
	/* Read the ADC values here */
		if(counter2 == 0){
			counter2++;
			a0_1 = Chip_ADC_GetDataReg(LPC_ADC0, 8);
			a10_1 = Chip_ADC_GetDataReg(LPC_ADC0, 10);
		}
		if(counter2 == 1){
			counter2++;
			a0_2 = Chip_ADC_GetDataReg(LPC_ADC0, 8);
			a10_2 = Chip_ADC_GetDataReg(LPC_ADC0, 10);
		}

		if(counter2 == 2){
			a0_3 = Chip_ADC_GetDataReg(LPC_ADC0, 8);
			a10_3 = Chip_ADC_GetDataReg(LPC_ADC0, 10);
			counter2 = 0;
		}

	}
	/* Clear any pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC0, pending);
}

}// extern "C"

// returns the interrupt enable state before entering critical section
bool enter_critical(void)
{
	uint32_t pm = __get_PRIMASK();
	__disable_irq();
	return (pm & 1) == 0;
}

// restore interrupt enable state
void leave_critical(bool enable)
{
	if(enable) __enable_irq();
}

// Example:
// bool irq = enter_critical();
// Change variables that are shared with an ISR
// leave_critical(irq);



void Sleep(uint32_t time)
{
	counter = time;
	while(counter > 0) {
		__WFI();
	}
}

int main(void) {
	bool blink = true ;
	int blinkUpdate = 0;
#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    //Board_LED_Set(0, true);
    ITM_init();
#endif
#endif

    // TODO: insert code here
	/* Setup ADC for 12-bit mode and normal power */
	Chip_ADC_Init(LPC_ADC0, 0);

	/* Setup for maximum ADC clock rate */
	Chip_ADC_SetClockRate(LPC_ADC0, ADC_MAX_SAMPLE_RATE);

	/* For ADC0, sequencer A will be used without threshold events.
	   It will be triggered manually, convert CH8 and CH10 in the sequence  */
	Chip_ADC_SetupSequencer(LPC_ADC0, ADC_SEQA_IDX, (ADC_SEQ_CTRL_CHANSEL(8) | ADC_SEQ_CTRL_CHANSEL(10) | ADC_SEQ_CTRL_MODE_EOS));

	/* For ADC0, select analog input pin for channel 0 on ADC0 */
	Chip_ADC_SetADC0Input(LPC_ADC0, 0);

	/* Use higher voltage trim for both ADC */
	Chip_ADC_SetTrim(LPC_ADC0, ADC_TRIM_VRANGE_HIGHV);

	/* Assign ADC0_8 to PIO1_0 via SWM (fixed pin) and ADC0_10 to PIO0_0 */
	Chip_SWM_EnableFixedPin(SWM_FIXED_ADC0_8);
	Chip_SWM_EnableFixedPin(SWM_FIXED_ADC0_10);

	/* Need to do a calibration after initialization and trim */
	Chip_ADC_StartCalibration(LPC_ADC0);
	while (!(Chip_ADC_IsCalibrationDone(LPC_ADC0)));

	/* Clear all pending interrupts and status flags */
	Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));

	/* Enable sequence A completion interrupts for ADC0 */
	Chip_ADC_EnableInt(LPC_ADC0, ADC_INTEN_SEQA_ENABLE);
	/* We don't enable the corresponding interrupt in NVIC so the flag is set but no interrupt is triggered */

	/* Enable related ADC NVIC interrupts */
	NVIC_EnableIRQ(ADC0_SEQA_IRQn);

	/* Enable sequencer */
	Chip_ADC_EnableSequencer(LPC_ADC0, ADC_SEQA_IDX);

	/* Configure systick timer */
	SysTick_Config(Chip_Clock_GetSysTickClockRate() / TICKRATE_HZ);

	//uint32_t a0;
	uint32_t d0;
	//uint32_t a10;
	uint32_t d10;
	int32_t diff;
	char str[80];
	//Sleep(100);

	while(1){

		// poll sequence complete flag
		//while(!(Chip_ADC_GetFlags(LPC_ADC0) & ADC_FLAGS_SEQA_INT_MASK));
		// clear the flags
		//Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));

		// get data from ADC channels
		/*a0 = Chip_ADC_GetDataReg(LPC_ADC0, 8); // raw value
		d0 = ADC_DR_RESULT(a0); // ADC result with status bits masked to zero and shifted to start from zero
		a10 = Chip_ADC_GetDataReg(LPC_ADC0, 10);
		d10 = ADC_DR_RESULT(a10);*/


		d0 = (ADC_DR_RESULT(a0_1) + ADC_DR_RESULT(a0_2) + ADC_DR_RESULT(a0_3))/3;
		d10 = (ADC_DR_RESULT(a10_1) + ADC_DR_RESULT(a10_2) + ADC_DR_RESULT(a10_3))/3;
		diff = d10 - d0;


		sprintf(str, " diff  = %d\n", diff);
		Board_UARTPutSTR(str);
		ITM_write(str);
		sprintf(str, " d0  = %lu\n", d0);
		Board_UARTPutSTR(str);
		ITM_write(str);
		sprintf(str, " d10 = %lu\n", d10);
		Board_UARTPutSTR(str);
		ITM_write(str);

		if (diff > 100){toggle = 0;}else if (diff < -10){toggle = 2;}
		if(-10 < diff && diff < 100){ ledRate = 500; toggle = 1;}//fast
		if(100 < diff && diff < 500){ ledRate = 36;}
		if(500 < diff && diff < 900){ ledRate = 31;}
		if(900 < diff && diff < 1400){ ledRate = 26;}
		if(1400 < diff && diff < 1800){ ledRate = 21;}
		if(1800 < diff && diff < 2200){ ledRate = 16;}
		if(diff > 2200){ ledRate = 6;}//slow

		if(-100 < diff && diff < -10){ ledRate = 36;}
		if(-400 < diff && diff < -100){ ledRate = 31;}
		if(-700 < diff && diff < -400){ ledRate = 26;}
		if(-1000 < diff && diff < -700){ ledRate = 21;}
		if(-1300 < diff && diff < -1000){ ledRate = 16;}
		if(diff < -1300){ ledRate = 6;}

		Sleep(100);

	}

    return 0 ;
}
