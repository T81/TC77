/*************************************************************************
* TC77 library
* by Christodoulos P. Lekkos <tolis81@gmail.com> , September 19, 2014.
*
* This file is free software; you can redistribute it and/or modify
* it under the terms of either the GNU General Public License version 3
* published by the Free Software Foundation.
*************************************************************************/

#ifndef TC77_H
#define TC77_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "SPI.h"

#define TC77_CELSIUS		0
#define TC77_KELVIN			1
#define TC77_FAHRENHEIT		2

#define TC77_DEBUG 			1	// set to 1 for debugging

class TC77
{
	public:
		TC77();
		void Begin(unsigned char csPin);
		bool Update(long interval = 300);
		signed long readRaw();
		float Temp(unsigned char unit = TC77_CELSIUS);
		void ID();
		void Config(bool mode);
	private:
	 unsigned long _previousMillis;
	 signed long _dataRaw;
	 unsigned char _csPin;
	 float _temp;
};
#endif 