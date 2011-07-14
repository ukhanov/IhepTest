/*
 * spi.cpp
 *
 *  Created on: 07.07.2011
 *      Author: Mikhail Ukhanov
 */
#include "../main.h"
#include "spi.h"
//-----------------------------------------------------------------------------

ISR(SPI_STC_vect)
{
	if(!(SPSR & _BV(WCOL)))
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
	/*
	 *
	 */
	SPI_DDR |= _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_SS);
	SPI_DDR &= ~_BV(SPI_MISO);
	/*
	 * Enable SPI interrupt,SPI, Master, MSB first,
	 * AD7791 requires the following conditions:
	 * CPOL = 1 CPHA = 1 :
	 * data is sampled on trailing edge of the SCLK
	 * and SCLK is HIGH when idle which makes SPI mode 3
	 *  set clock rate fck/16
	 */
	SPCR = _BV(SPIE)|_BV(SPE)|_BV(MSTR)|_BV(CPOL)|_BV(CPHA)|_BV(SPR0);
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
#if 0
/*
 * THIS IS WRONG CODE !!!!!
 * It will hang up if nobody respond.
 * It is strongly recommended to use interrupt mechanism.
 * The data must be read in the main event loop.
 */
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
#endif
//-----------------------------------------------------------------------------

void SPI_Write(uint8_t cData)
{
uint8_t SPI_ief;
	/* Disable the SPI interrupt since it
	 *  will (could?) be triggered with this transfer.
	 */
	SPI_ief = SPCR &_BV(SPIE); // Save status of interrupt enable bit.
	SPCR &= ~_BV(SPIE);
	SPDR = cData;
	/* Start transmission */
	SS_DOWN(); // Pull down the SS signal
	/* Wait for transmission complete */
	while(!(SPSR & _BV(SPIF)));
	/*
	 * Restore the status of the interrupt enable bit
	 * in the SPI control register
	 */
	intflags.spi_int = 0;
	SPCR |= SPI_ief;
}
//-----------------------------------------------------------------------------
