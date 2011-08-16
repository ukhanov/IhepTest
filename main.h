/*
 * main.h
 *
 *  Created on: 27.06.2011
 *      Author: M.Ukhanov
 */
#include <avr/io.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/wdt.h>

#ifndef MAIN_H_
#define MAIN_H_

// SOftware timers interface
#define TIMER_FLAG	0xFFFF
#define TIC_TIMER(timer)	{ if(timer) timer--; }
#define SET_TIMER(timer,Tms)	{ timer = 1+Tms/SYS_TICms;}
typedef uint16_t SWtimer_t;

// Global variables
#ifdef MAIN_IMPLEMENTATION_
#define GLOB_EXTERN
#else
#define GLOB_EXTERN extern
#endif
GLOB_EXTERN volatile uint32_t systime;
GLOB_EXTERN volatile uint8_t rs232buf,rs485buf,spibuf;                   /* Last character read from the UART. */
GLOB_EXTERN volatile struct INT_FLAGS {
  uint8_t tmr_int	:	1;
  uint8_t adc_int	:	1;
  uint8_t rx232_int	:	1;
  uint8_t rx485_int	:	1;
  uint8_t spi_int	:	1;
}
intflags;
GLOB_EXTERN volatile SWtimer_t KbdTimer,SYS_TICms;

// Function prototypes
void io_init(void);
#define NOP() asm("NOP")
#endif /* MAIN_H_ */
