/*
 * rs485.h
 *
 *  Created on: 28.06.2011
 *      Author: 1
 */

#ifndef RS485_H_
#define RS485_H_

//    RS485 write enable
#define PIN_WRITE_ENABLE 	PE3	// OC3A / AIN1 //ACSR |= _BV(ACD); // AIN1: ACD = Analog Comparator Disable //TCCR3A &= ~(_BV(COM3A0) | _BV(COM3A1)); // OC3A: Disable TIMER3 output compare
#define PORT_WRITE_ENABLE 	PORTE
#define DDR_WRITE_ENABLE 	DDRE
#define PINS_WRITE_ENABLE	PINE

// RS-485 half duplex helpers
#define RS485TX_OFF() {								\
	_delay_ms(5);									\
	PORT_WRITE_ENABLE &= ~(_BV(PIN_WRITE_ENABLE)); 	\
}

#define RS485TX_ON() {								\
	PORT_WRITE_ENABLE |= _BV(PIN_WRITE_ENABLE); 	\
}

void InitRs485(void);
#endif /* RS485_H_ */
