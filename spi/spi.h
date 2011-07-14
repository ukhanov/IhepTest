/*
 * spi.h
 *
 *  Created on: 07.07.2011
 *      Author: Mikhail Ukhanov
 */

#ifndef SPI_H_
#define SPI_H_

#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define SPI_MOSI	PB2
#define SPI_SCK		PB1
#define SPI_MISO	PB3
#define SPI_SS		PB0
#define SS_DOWN()	{SPI_PORT &= ~_BV(SPI_SS);}
//-----------------------------------------------------------------------------

void InitSpiMaster(void);
void InitSpiSlave(void);
// See file spi.cpp for explanation on 'uint8_t SPI_SlaveReceive(void);'
void SPI_Write(uint8_t cData);

#endif /* SPI_H_ */
