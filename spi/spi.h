/*
 * spi.h
 *
 *  Created on: 07.07.2011
 *      Author: Mikhail Ukhanov
 */

#ifndef SPI_H_
#define SPI_H_

void InitSpiMaster(void);
void InitSpiSlave(void);
uint8_t SPI_SlaveReceive(void);
void SPI_MasterTransmit(uint8_t cData);

#endif /* SPI_H_ */
