/*
 * main.cc
 *
 *  Created on: 27.06.2011
 *      Author: M.Ukhanov
 */

#define DEBUG
#define MAIN_IMPLEMENTATION_

#include "main.h"
#include "adc/adc.h"
#include "gpio/gpio.h"
#include "i2c/i2c.h"
#include "spi/spi.h"
#include "rs232/rs232.h"
#include "rs485/rs485.h"
#include "utility/utility.h"

//-----------------------------------------------------------------------------

uint8_t mcucsr;
/*
 * Read out and reset MCUCSR early during startup.
 */
void handle_mcucsr(void)
__attribute__((section(".init3")))
__attribute__((naked));
void handle_mcucsr(void) {
	mcucsr = MCUCSR;
	MCUCSR = 0;
}
//-----------------------------------------------------------------------------
PGM_P Oops = "\nOoops, the watchdog bits me!";
PGM_P Hello = "\nHello, this is ABTOMATNKA controller running on an ATmega 128";
int main(void) {
	uint8_t buf[3];
	io_init();

	if ((mcucsr & _BV(WDRF)) == _BV(WDRF)) {
#ifdef DEBUG
		printstr_p(Oops);
#endif
	}
	printstr_p(Hello);
	PORTC = 0;
	for (;;) { // Main event loop
		wdt_reset();
#if 1
		// Send meander to all outputs
		PORTC ^= 0xFF;
		PORTA ^= 0xF0;
		//for(uint8_t i=0; i<80; i++) ;
		_delay_us(50); // wait half a second
		// putchr('C');
#else

		for(uint8_t i=0; i<3; i++) { // read status of the input registers
			PORTA &=0xF8; // clear chip select
			PORTA |= 1<<i; // Select the register
			PORTA |= 0x08; // Set clock pin high
			PORTA &= ~0x08; // Pull down the clock pin
			uint8_t c = PINF; // Read the result
			if(c != buf[i]) {
				putchr('\n');
				putchr('\r');
				putchr('0'+i);
				putchr(':');
				printbin(c);
			}
			buf[i] = c;
		}
		//#else

		if (intflags.tmr_int) { // System clock interrupt
			intflags.tmr_int = 0;
		}
		if (intflags.adc_int) { // Remnants of other project (should never happen)
			intflags.adc_int = 0;
		}
		if (intflags.rx232_int) { // RS232 character received
			intflags.rx232_int = 0;
		}
		if (intflags.rx485_int) { // RS485 character received
			intflags.rx485_int = 0;
		}
		sleep_mode(); // It is supposed to be a major occupation
#endif
	}
	return 0;
}
//-----------------------------------------------------------------------------------------

void io_init(void) {
	// Init all periphery here

	// Disable all timer interrupts
	TIMSK = 0;
	// Use full quartz speed
	XDIV = 0;

	InitGpio();
	InitRs485();
	InitRs232();
	InitI2c();
	InitSpi();

	// At last init timers etc...
	InitUtility();

	sei();
	/* enable interrupts */

	/*
	 * Enable the watchdog with the largest prescaler.  Will cause a
	 * watchdog reset after approximately 2 s @ Vcc = 5 V
	 */wdt_enable(WDTO_2S);
}
//-----------------------------------------------------------------------------------------
