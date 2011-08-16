/*
 * rs232.cpp
 *
 *  Created on: 28.06.2011
 *      Author: 1
 */
#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "../main.h"
#include "rs232.h"

//-----------------------------------------------------------------------------
static void
SetUart1Baud(void)
{
#define BAUD 9600
#include <util/setbaud.h>
   UBRR1H = UBRRH_VALUE;
   UBRR1L = UBRRL_VALUE;
#if USE_2X
   UCSR1A |= (1 << U2X1);
#else
   UCSR1A &= ~(1 << U2X1);
#endif
}
//-----------------------------------------------------------------------------

void InitRs232(void)
{
/*
 * Set baud rate of USART1 port
 * Set 8-N-1
 */
	UCSR1A = 0;
	UCSR1B = _BV(RXCIE1) | _BV(TXEN1) | _BV(RXEN1); // Enable RX,TX and RX complete interrupts.
	UCSR1C = _BV(UCSZ11) | _BV(UCSZ10);// | _BV(USBS1); // Asynchronous, No parity, 8 bit mode
	UBRR1L = 25; UBRR1H = 0;//SetUart1Baud(); 25->19200
}
//---------------------------------------------------------------------------
#if 1

/*
 * UART receive interrupt.  Fetch the character received and buffer
 * it, unless there was a framing error.
 */
ISR(USART1_RX_vect)
{
	if (bit_is_clear(UCSR1A, FE)) // No Frame_Error was detected.
	{
		rs232buf = UDR1;
		intflags.rx232_int = 1;
	} else {
		rs232buf = 0;
	}
}
//---------------------------------------------------------------------------
/*
 * Some simple UART IO functions.
 */

/*
 * Send character c down the UART Tx, wait until tx holding register
 * is empty.
 *  Turn the receiver off to supress the echo.
 */
void putchr(uint8_t c)
{
//	UCSR1B &= ~(_BV(RXEN1)|_BV(RXCIE));/*  Turn the receiver off to supress the echo. */
	loop_until_bit_is_set(UCSR1A, UDRE1);
	UDR1 = c;
//	loop_until_bit_is_set(UCSR1A, UDRE1);
	intflags.rx232_int = 0;
//	UCSR1B |= _BV(RXEN1)|_BV(RXCIE1); // Enable the RX again
}
//---------------------------------------------------------------------------

/*
 * Send a C (NUL-terminated) string down the UART Tx.
 */

void printstr(const char *s)
{
return;

  while (*s)
    {
      if (*s == '\n')
		putchr('\r');
      putchr(*s++);
    }
}
//---------------------------------------------------------------------------

/*
 * Same as above, but the string is located in program memory,
 * so "lpm" instructions are needed to fetch it.
 */
void printstr_p(PGM_P s)
{
char c;

  for (c = pgm_read_byte(s); c; ++s, c = pgm_read_byte(s))
    {
      if (c == '\n')
			putchr('\r');
      putchr(c);
    }
}
//---------------------------------------------------------------------------
#endif

