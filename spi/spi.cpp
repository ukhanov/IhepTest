/*
 * spi.cpp
 *
 *  Created on: 11.08.2011
 *      Author: Mikhail Ukhanov
 *  The SPI protocol is implemented 'by hand' using the same pins
 *  as the native Atmel implementation.
 *  The reason to not use the original ATmega128 SPI is my inability to
 *  read a byte from slave (Who will supply clocks on SCK?).
 *  I think that it is my incompetence and not the fault of Atmel.
 */
#include "../main.h"
#include "spi.h"
//-----------------------------------------------------------------------------

void InitSpiMaster()
{
	/*
	 * Init SPI master
	 */
/* Disable native SPI */
//	SPCR &= ~_BV(SPE);

/* Set MOSI, SS and SCK output, MISO input */
	SPI_DDR |= _BV(SPI_MOSI) | _BV(SPI_SCK);// | _BV(SPI_SS); SS is not wired
	SPI_DDR &= ~_BV(SPI_MISO);
}
//-----------------------------------------------------------------------------

void SPI_Write(uint8_t cData)
{
	/* Just transmit a byte.
	 * The overall slave synchronization should
	 * be done in the calling function.
	 */
uint8_t i;
/* We assume that slave samples the MOSI data on the rising edge of SCK */
	SPI_SCK_HIGH();
/* Start transmission MSB first */
	for(i=0; i<8; i++) {
		SPI_DELAY();
		SPI_SCK_LOW();
		if( cData & (1<<(7-i)) )
			{MOSI_HIGH();}
		else
			{MOSI_LOW();}
		SPI_DELAY();
		SPI_SCK_HIGH();
	}
}
//-----------------------------------------------------------------------------
uint8_t SPI_Read()
{
	/* Just read what ever comes on MISO.
	 * The overall slave synchronization should be done
	 * in the calling function.
	 */
uint8_t i,buf;
	buf = 0;
/* We assume that the slave sets the data on the rising edge of SCK */
	SPI_SCK_HIGH(); // slave transmission synchronization
/* Start reception MSB first */
	for(i=0; i<8; i++) {
		SPI_DELAY();
		SPI_SCK_LOW();
		SPI_DELAY();
		SPI_SCK_HIGH();
		if(SPI_PINS & _BV(SPI_MISO)){
			buf |= 1<<(7-i);
		}
	}
	return buf;
}
//-----------------------------------------------------------------------------
