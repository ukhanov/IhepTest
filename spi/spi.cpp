/*
 * spi.cpp
 *
 *  Created on: 07.07.2011
 *      Author: Mikhail Ukhanov
 */
#include "../main.h"
#include "spi.h"
//-----------------------------------------------------------------------------
#define SPI_DDR		DDRB
#define SPI_MOSI	PB2
#define SPI_SCK		PB1
#define SPI_MISO	PB3
#define SPI_SS		PB0

//-----------------------------------------------------------------------------

ISR(SPI_STC_vect)
{
	if(!(SPSR & _BV(WCOL))
			intflags.spi_int = 1;
	spibuf = SPDR;  // The data register should always be read to ensure the bit WCOL clearance
}
//-----------------------------------------------------------------------------

void InitSpiMaster()
{
	/*
	 * Init SPI master
	 */

	/* Set MOSI, SS and SCK output, MISO input */
	SPI_DDR |= _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_SS);
	SPI_DDR &= ~_BV(SPI_MISO);
	/*
	 * Enable SPI interrupt,SPI, Master, MSB first,
	 * SPI mode 0, set clock rate fck/16
	 */
	SPCR = _BV(SPIE)|_BV(SPE)|_BV(MSTR)|_BV(SPR0);
	SPSR = 0;

}
//-----------------------------------------------------------------------------

void InitSpiSlave(void)
{
/* Set MISO output, MOSI and SCK input */
	SPI_DDR |= _BV(SPI_MISO);
	SPI_DDR &= ~(_BV(SPI_MOSI)|_BV(SPI_SCK) | _BV(SPI_SS));
	/*
	 * Enable SPI interrupt,SPI, Slave, MSB first,
	 * SPI mode 0, set clock rate fck/16
	 */
	SPCR = _BV(SPIE)|_BV(SPE)|_BV(SPR0);
	SPSR = 0;
}
//-----------------------------------------------------------------------------

uint8_t SPI_SlaveReceive(void)
{
uint8_t SPI_ief,data;
	/* First disable the SPI interrupt
	 * since it will be triggered with this transfer.
	 */
	SPI_ief = SPCR &_BV(SPIE);
	SPCR &= ~_BV(SPIE);

/* Wait for reception complete */
	while(!(SPSR & _BV(SPIF)))
		;
/* Return data register */
	data = SPDR;
	/*
	 * Restore the status of the interrupt enable bit
	 * in the SPI control register
	 */
	if(SPI_ief) SPCR |= _BV(SPIE);
	return data;
}
//-----------------------------------------------------------------------------

void SPI_MasterTransmit(uint8_t cData)
{
uint8_t SPI_ief;
	/* First disable the SPI interrupt
	 * since it will be triggered with this transfer.
	 */
	SPI_ief = SPCR &_BV(SPIE);
	SPCR &= ~_BV(SPIE);
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & _BV(SPIF)));
	/*
	 * Restore the status of the interrupt enable bit
	 * in the SPI control register
	 */
	if(SPI_ief) SPCR |= _BV(SPIE);
}
//-----------------------------------------------------------------------------
