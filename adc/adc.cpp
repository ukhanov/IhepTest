/*
 * adc.cpp
 *
 *  Created on: 28.06.2011
 *      Author: M.Ukhanov
 */
#include "../main.h"
#include "../spi/spi.h"
#include "../rs232/rs232.h"
#include "../utility/utility.h"
#include "adc.h"

#define HIGH_OE_ADC()		{PORTE |= ADC_OE_ADC;}
#define LOW_OE_ADC()		{PORTE &= ~ADC_OE_ADC;}
#define HIGH_CS_ADC()		{PORTB |= ADC_CS_ADC;}
#define LOW_CS_ADC()		{PORTB &= ~ADC_CS_ADC;}
#define SET_ADC_ADR(NADC)	{PORTE &= ~(7<<5); PORTE |=  (NADC<<5);}
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
	HIGH_OE_ADC();
	SET_ADC_ADR(nADC);
	SPI_DELAY();
	HIGH_CS_ADC(); // Start of SPI protocol
	SPI_Write(ADC_READ);
#if 0
	SPI_DELAY();
	HIGH_CS_ADC(); // End of SPI protocol
	SPI_DELAY();
	LOW_CS_ADC(); // Start of SPI protocol
#endif
	spibuf = SPI_Read();
	LOW_CS_ADC(); // End of SPI protocol
	LOW_OE_ADC();
}
//-----------------------------------------------------------------------------
void ADC_Data(uint8_t nADC, uint8_t Ch)
{
/*
 * Read data from ADC channel nADC
 */
uint8_t tmp;
char *ptr, cbuf[10];
int32_t adcval;
#define NTRIES 250

	ptr = (char *)&adcval;
	adcval = 0;
	spibuf = 0;
	HIGH_OE_ADC();
	SET_ADC_ADR(nADC);
	putchr(nADC+'1'); putchr(' ');

	SPI_DELAY();
	HIGH_CS_ADC(); // Start of SPI protocol
	SPI_Write(ADC_MODE | Ch);
	LOW_CS_ADC(); // End of SPI protocol

	SPI_DELAY();
	HIGH_CS_ADC(); // Start of SPI protocol
	SPI_Write(ADC_UNIPOLAR);
	LOW_CS_ADC(); // End of SPI protocol

	for(tmp=0; tmp < NTRIES; tmp++) {
		ADC_Status(nADC);
		if(!(spibuf & 0x80) && !(spibuf&0x40)) break;
		_delay_ms(2);
	}
	if(tmp == NTRIES){
		printstr_p(PSTR("ADC error "));
		printbin(spibuf);
		putchr('\n');putchr('\r');
		return;
	}
	HIGH_OE_ADC();  // Since ADC_status clears it
	SET_ADC_ADR(nADC);
	SPI_DELAY();
	HIGH_CS_ADC(); // Start of SPI protocol
	SPI_Write(ADC_READ | ADC_DATA | Ch);
	LOW_CS_ADC(); // End of SPI protocol

	SPI_DELAY();
	HIGH_CS_ADC(); // Start of SPI protocol
	for(tmp = 0; tmp<3; tmp++) {
		ptr[2-tmp] = SPI_Read();
	}
	LOW_CS_ADC(); // End of SPI protocol
	LOW_OE_ADC();
	printbin(spibuf);

	printstr_p(PSTR("\nAmplitude: "));
//	ltoa(adcval,cbuf,10);
	printdec(adcval);
	for(tmp=0; tmp<3; tmp++) {
		putchr(' ');
		printbin(ptr[2-tmp]);
	}
	putchr('\n');putchr('\r');

}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
