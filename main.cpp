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
#define DUMP_MEM_ADR(MEMADR,ADRSTR){	\
	printstr(ADRSTR);					\
	tmp = 1 + (uint8_t*)MEMADR;			\
	printbin(*tmp);putchr(' '); tmp--;	\
	printbin(*tmp);putchr(' ');			\
	r = (int)tmp;						\
	adr = *(((uint8_t*)&r)+1);			\
	printbin(adr);putchr(' ');			\
	adr = *((uint8_t*)&r);				\
	printbin(adr);putchr(' ');			\
}

void dumpmem()
{
uint8_t *tmp,adr;
int r;
	DUMP_MEM_ADR(0x3C,"0x3C ");
	DUMP_MEM_ADR(0x3E,"\n0x3E ");
	DUMP_MEM_ADR(0x40,"\n0x40 ");
	DUMP_MEM_ADR(0x24,"\n\n0x24 ");
	DUMP_MEM_ADR(0x26,"\n0x26 ");
	DUMP_MEM_ADR(0x28,"\n0x28 ");
	printstr("\n");
}
//-----------------------------------------------------------------------------
#define NLCR() {putchr('\n');putchr('\r');}
#define TTY_TICK 50
int main(void)
{
uint8_t buf[3];
uint16_t tick;

	io_init();

	if ((mcucsr & _BV(WDRF)) == _BV(WDRF)) {
		printstr_p(PSTR("\nOoops, the watchdog bits me!\n"));
	}
	printstr_p(PSTR("\nHello, this is ABTOMATNKA controller running on ATmega 128\n"));
	PORTC = 0;
//	dumpmem();
	for (;;) { // Main event loop
		wdt_reset();
#if 1
		// Monitor changes of optoisolated input
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
#endif
		if (intflags.tmr_int) { // System clock interrupt
			intflags.tmr_int = 0;
			if(++tick%TTY_TICK == 0) {
#if 0
				RS485TX_ON();
				putchr485('C');
				RS485TX_OFF();
#endif
//				putchr('.');
//				if(tick%(50*TTY_TICK) == 0)NLCR();
				PORTC ^= 0xFF;
				PORTA ^= 0xF0;
			}
		}
		if (intflags.adc_int) { // Remnants of other project (should never happen)
			intflags.adc_int = 0;
		}
		if (intflags.rx232_int) { // RS232 character received
			intflags.rx232_int = 0;
			putchr('\n');	putchr('\r');
			putchr(rs232buf);
			putchr('\n');	putchr('\r');

			if(rs232buf > '0' && rs232buf < '9') {
				ADC_Status(rs232buf-'1');
				printbin(spibuf);
				putchr('\n');putchr('\r');
			} else if (rs232buf >= 'a' && rs232buf <= 'h') {
				ADC_Data(rs232buf-'a',ADC_VDD);
			} else if (rs232buf >= 'A' && rs232buf <= 'H') {
				ADC_Data(rs232buf-'A',0);
			} else {
				RS485TX_ON();
				putchr485(rs232buf);
				RS485TX_OFF();
			}
		}
		if (intflags.rx485_int) { // RS485 character received
			intflags.rx485_int = 0;
			putchr(rs485buf);  // echo the char received from RS485
			if((rs485buf != '?') && (rs485buf != rs232buf)){
				RS485TX_ON();
				putchr485(rs485buf);
				RS485TX_OFF();
			}
			rs232buf = 0;
		}
		if (intflags.spi_int) { // SPI character received
			intflags.spi_int = 0;
			printstr_p(PSTR("SPI error!\n "));
			SS_HIGH();
			printbin(spibuf);
			putchr('\n');putchr('\r');
		}
		sleep_mode(); // It is supposed to be a major occupation
	}
	return 0;
}
//-----------------------------------------------------------------------------------------

void io_init(void) {
/*
 *  Init all periphery here
 *  NOTE!!! Order is important
 */
	// Disable all timer interrupts
	TIMSK = 0;
	ETIMSK = 0;
	// Use full quartz speed
	XDIV = 0;

	InitGpio();
	InitRs232();
	InitRs485();
	InitI2c();
	InitSpiMaster();
	InitAdc();
	InitUtility(); // At last init timers etc...

	/* enable interrupts */
	sei();

	/*
	 * Enable the watchdog with the largest prescaler.  Will cause a
	 * watchdog reset after approximately 2 s @ Vcc = 5 V
	 */
	wdt_enable(WDTO_2S);
}
//-----------------------------------------------------------------------------------------
