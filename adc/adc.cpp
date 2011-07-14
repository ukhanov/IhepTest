/*
 * adc.cpp
 *
 *  Created on: 28.06.2011
 *      Author: M.Ukhanov
 */
#include "../main.h"
#include "../spi/spi.h"
#include "adc.h"

#define SET_OE_ADC()		{PORTE |= ADC_OE_ADC;}
#define CLEAR_OE_ADC()		{PORTE &= ~ADC_OE_ADC;}
#define SET_CS_ADC()		{PORTB |= ADC_CS_ADC;}
#define CLEAR_CS_ADC()		{PORTB &= ~ADC_CS_ADC;}
#define SET_ADC_ADR(nADC)	{PORTE |=  (nADC<<5);}
//-----------------------------------------------------------------------------

void InitAdc(void)
{
	// Set output pins

	DDRE |= ADC_OE_ADC |_BV(PE5)|_BV(PE6)|_BV(PE7);// Signals are: OE-ADC, A, B, C
	DDRB |= ADC_CS_ADC;		// CS-ADC

}
//-----------------------------------------------------------------------------

void ADC_Status(uint8_t nADC)
{
/*
 * Read status of ADC channel nADC
 */
uint8_t tmp;
	SET_ADC_ADR(nADC);
	SET_OE_ADC();
	SET_CS_ADC();
	tmp = ADC_READ;
	spibuf = 0;
	SPI_Write(tmp);
	CLEAR_CS_ADC(); // Signaling to AD7791 end of transmission
	asm("NOP");
	// initialize reading from AD7791
	SET_CS_ADC();
	SS_DOWN(); // Pull down the SS signal
}
//-----------------------------------------------------------------------------
void ADC_Data(uint8_t nADC)
{
/*
 * Read data from ADC channel nADC
 */
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
