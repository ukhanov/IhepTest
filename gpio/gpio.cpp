/*
 * gpio.cpp
 *
 *  Created on: 28.06.2011
 *      Author: M.Ukhanov
 */
#include "../main.h"
#include "gpio.h"

//-----------------------------------------------------------------------------

void InitGpio(void)
{
	DDRA = 0xFF;
	DDRC = 0xFF;
	DDRF = 0;    // Registers input
}
//-----------------------------------------------------------------------------

