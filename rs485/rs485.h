/*
 * rs485.h
 *
 *  Created on: 28.06.2011
 *      Author: Mikhail Ukhanov
 */

#ifndef RS485_H_
#define RS485_H_

//    RS485 write enable
#define PIN_485_WE 	PE3	// OC3A / AIN1 //ACSR |= _BV(ACD); // AIN1: ACD = Analog Comparator Disable //TCCR3A &= ~(_BV(COM3A0) | _BV(COM3A1)); // OC3A: Disable TIMER3 output compare
#define PORT_485_WE	PORTE
#define DDR_485_WE 	DDRE
#define PINS_485_WE	PINE

// RS-485 half duplex helpers
#define RS485TX_OFF() {					\
	_delay_ms(1);						\
	PORT_485_WE &= ~(_BV(PIN_485_WE)); 	\
}

#define RS485TX_ON() {					\
	PORT_485_WE |= _BV(PIN_485_WE); 	\
	_delay_ms(1);						\
}

void InitRs485(void);
void putchr485(uint8_t c);

#endif /* RS485_H_ */
