/*
 * adc.h
 *
 *  Created on: 28.06.2011
 *      Author: 1
 */

#ifndef ADC_H_
#define ADC_H_

/* Definition of the Communication register control bits value */
#define ADC_READ	(1<<3)
#define ADC_WRITE	0
#define ADC_CREAD	(1<<2)
#define ADC_DIFF	0
#define ADC_PED		2
#define ADC_VDD		3
#define ADC_STATUS	0
#define ADC_MODE	(1<<4)
#define ADC_FILTER	(1<<5)
#define ADC_DATA	(3<<4)
#define ADC_OE_ADC	_BV(PE2)
#define ADC_CS_ADC	_BV(PB4)

void InitAdc(void);
void ADC_Status(uint8_t c);
void ADC_Data(uint8_t c);

#endif /* ADC_H_ */
