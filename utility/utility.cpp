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
	PORTB ^= _BV(PB1);
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
