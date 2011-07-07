/*
 * rs485.cpp
 *
 *  Created on: 28.06.2011
 *      Author: 1
 */
#include "../main.h"
#include "rs485.h"

//-----------------------------------------------------------------------------

void InitRs485(void)
{

}
//-----------------------------------------------------------------------------
/*
 * Send character c down the UART0 Tx, wait until tx holding register
 * is empty.
 *  Turn the receiver off to suppress the echo.
 */
void putchr485(uint8_t c)
{
	UCSR0B &= ~(_BV(RXEN)|_BV(RXCIE));
	loop_until_bit_is_set(UCSR0A, UDRE);
	UDR0 = c;
	loop_until_bit_is_set(UCSR0A, UDRE);
	intflags.rx485_int = 0;
	UCSR0B |= _BV(RXEN)|_BV(RXCIE);
}
//---------------------------------------------------------------------------
