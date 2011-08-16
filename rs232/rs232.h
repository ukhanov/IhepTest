/*
 * rs232.h
 *
 *  Created on: 28.06.2011
 *      Author: 1
 */

#ifndef RS232_H_
#define RS232_H_

void InitRs232(void);
void putchr(uint8_t c);
void printstr(const char *s);
void printstr_p(PGM_P s);
#endif /* RS232_H_ */
