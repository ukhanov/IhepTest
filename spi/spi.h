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
#define SPI_PINS	PINB
#define SPI_MOSI	PB2
#define SPI_SCK		PB1
#define SPI_MISO	PB3
#define SPI_SS		PB0
#define SS_LOW()	{SPI_PORT &= ~_BV(SPI_SS);}
#define SS_HIGH()	{SPI_PORT |= _BV(SPI_SS);}
#define SPI_SCK_LOW()	{SPI_PORT &= ~_BV(SPI_SCK);}
#define SPI_SCK_HIGH()	{SPI_PORT |= _BV(SPI_SCK);}
#define MOSI_LOW()	{SPI_PORT &= ~_BV(SPI_MOSI);}
#define MOSI_HIGH()	{SPI_PORT |= _BV(SPI_MOSI);}
#define SPI_DELAY()	_delay_loop_1(5)
//-----------------------------------------------------------------------------

void InitSpiMaster(void);
void SPI_Write(uint8_t cData);
uint8_t SPI_Read();

#endif /* SPI_H_ */
