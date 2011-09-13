/*
 * rs485.cpp
 *
 *  Created on: 28.06.2011
 *      Author: Mikhail Ukhanov
 */
#include "../main.h"
#include "rs485.h"

//-----------------------------------------------------------------------------
/*
 * UART receive interrupt.  Fetch the character received and buffer
 * it, unless there was a framing error.
 */
ISR(USART0_RX_vect)
{
	cli();
	if (bit_is_clear(UCSR0A, FE)) // No Frame_Error was detected.
	{
		rs485buf = UDR0;
		intflags.rx485_int = 1;
	} else {
		rs485buf = 0;
	}
	sei();
}
//---------------------------------------------------------------------------

void InitRs485(void)
{
	DDR_485_WE |= _BV(PIN_485_WE); // Enable output of the WRITE_ENABLE pin of the ADM485
	NOP();NOP();NOP();_delay_ms(1);
	RS485TX_OFF();
/*
 * Set baud rate of USART1 port
 * Set 8-N-1
 */
	UCSR0A = 0;
	UCSR0B = _BV(RXCIE0) | _BV(TXEN0) | _BV(RXEN0); // Enable RX,TX and RX complete interrupts.
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);// | _BV(USBS1); // Asynchronous, No parity, 8 bit mode
	UBRR0L = 25; UBRR0H = 0;//SetUart1Baud(); 25->19200

}
//-----------------------------------------------------------------------------
/*
 * Send character c down the UART0 Tx, wait until tx holding register
 * is empty.
 *  Turn the receiver off to suppress the echo.
 */
void putchr485(uint8_t c)
{
//	UCSR0B &= ~(_BV(RXEN0)|_BV(RXCIE0));
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	loop_until_bit_is_set(UCSR0A, TXC0);
	intflags.rx485_int = 0;
//	UCSR0B |= _BV(RXEN0)|_BV(RXCIE0);
}
//---------------------------------------------------------------------------

/*
 * Send a C (NUL-terminated) string down the UART Tx.
 */

void printstr485(const char *s)
{
  while (*s)
    {
      if (*s == '\n')
		putchr485('\r');
      putchr485(*s++);
    }
}
//---------------------------------------------------------------------------

/*
 * Same as above, but the string is located in program memory,
 * so "lpm" instructions are needed to fetch it.
 */
void printstr485_p(const char *s)
{
char c;

  for (c = pgm_read_byte(s); c; ++s, c = pgm_read_byte(s))
    {
      if (c == '\n')
			putchr485('\r');
      putchr485(c);
    }
}
//---------------------------------------------------------------------------
