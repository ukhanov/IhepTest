/*
 * utility.cpp
 *
 *  Created on: 28.06.2011
 *      Author: M.Ukhanov
 */
#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "../main.h"
#include "../rs232/rs232.h"
#include "utility.h"
//---------------------------------------------------------------------------------------

void InitUtility()
{ // Setup all three timers here
	// Timer0  - 8-bit system clock timer
	// Normal mode, output pins are disconnected, use 1024 prescaler.
	// It gives 125 ms tick.
	TCCR0 = _BV(CS02) | _BV(CS00);
	TIMSK |= _BV(TOIE0);
	SYS_TICms = 125;
	// Timer1  - keyboard scan
	// Timer2  - LCD running message

}
//---------------------------------------------------------------------------

ISR(TIMER0_OVF_vect)
{
	intflags.tmr_int = 1;
}
//---------------------------------------------------------------------------

ISR(TIMER1_OVF_vect)
{
}
//---------------------------------------------------------------------------

ISR(TIMER2_COMP_vect)
{
}
//---------------------------------------------------------------------------

void printbin(uint8_t c)
{
	for(uint8_t i=0; i<8; i++) {
		uint8_t d = ((0x80>>i)&c) ? '1' : '0';
		putchr(d);
	}
}
//-----------------------------------------------------------------------------

void printdec(uint32_t val)
{
const uint32_t BigVal = 1000000000;
	for(uint32_t b = BigVal; b > 0; b /=10) {
		uint8_t c = val/b;
		putchr(c+'0');
		val -= c*b;
	}
}
//-----------------------------------------------------------------------------
