/*************************************************************************
* TC77 library
* by Christodoulos P. Lekkos <tolis81@gmail.com> , September 03, 2014.
*
* http://www.microchip.com/wwwproducts/Devices.aspx?product=TC77

* This file is free software; you can redistribute it and/or modify
* it under the terms of either the GNU General Public License version 3
* published by the Free Software Foundation.
*************************************************************************/

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <TC77.h>

TC77::TC77() {
}

void TC77::Begin(unsigned char csPin) {
	_csPin = csPin;
	pinMode(_csPin, OUTPUT);		// set CS pin as output
	digitalWrite(_csPin, HIGH);		// deselect the device
	_previousMillis = millis();		// initialize clock
}

bool TC77::Update(long interval) {
	interval <= 300 ? 300 : interval;								// min time between conversions 300ms (datasheet p.2)
	unsigned long timePassed = millis() - _previousMillis;			// store time passed since last check
	if (timePassed >= interval) {									// if settle time has passed				
		digitalWrite(_csPin, LOW);									// select TC77
		_dataRaw = (SPI.transfer(0x00) << 8) | SPI.transfer(0x00);	// get the 2 bytes
		digitalWrite(_csPin, HIGH); 								// Set CS to HIGH to complete measurement
		_previousMillis = millis();									// reinitialize clock
		_dataRaw = (_dataRaw >> 3);									// get rid the first 3 bits
		// _dataRaw = (0xFFFF >> 3) & 0x1FFF;							// uncomment for debug
		if (_dataRaw & 0x1000) {									// in cases of negative temperature									
			_dataRaw = (_dataRaw & 0xFFF) - 4096;
		}
		return true;												// return a successful update
	}
	else {															// value not updated
		return false;
	}
}

signed long TC77::readRaw() {
	return _dataRaw;
}

float TC77::Temp(unsigned char unit) {
	_temp = (float)(_dataRaw)*0.0625;
	if (unit == TC77_CELSIUS) {
		return _temp;
	}
	else if (unit == TC77_KELVIN) {
		return (_temp + 273.0);	
	}
	else if (unit == TC77_FAHRENHEIT) {
		return (_temp * 9/5 + 32);
	}
	else {
		return 0xFFFF;
	}
}

void TC77::ID() {
	digitalWrite(_csPin, LOW);
	unsigned char incomingByte = 0;
	SPI.transfer(0x00);					// temperature byte
	SPI.transfer(0x00);					// temperature byte
	SPI.transfer(0xFF);					// enter shutdown mode
	SPI.transfer(0xFF);					// enter shutdown mode
	incomingByte = SPI.transfer(0xFF);	// Manufacturer's ID (C15:C8)
	Serial.println(incomingByte, HEX);
	SPI.transfer(0xFF);					// Manufacturer's ID (C7:C0)
	SPI.transfer(0x00);					// enter Continuous Conversion Mode
	SPI.transfer(0x00);					// enter Continuous Conversion Mode
	digitalWrite(_csPin, HIGH);
	#if TC77_DEBUG
		Serial.print("TC77 @ CS pin ");
		Serial.print(_csPin);
	#endif
	if (incomingByte == 0x54) { 
		#if TC77_DEBUG
			Serial.println(" identified.");
		#endif
	}
	else {	// something goes wrong
		#if TC77_DEBUG
			Serial.println(" not identified");
		#endif
		while(1) {	// halt
		}
	}	
}

void TC77::Config(bool mode) {
	static bool shutdownMode = false;
	if (!shutdownMode && mode) {
		shutdownMode = true;
		digitalWrite(_csPin, LOW);
		SPI.transfer(0x00);					// temperature byte
		SPI.transfer(0x00);					// temperature byte
		SPI.transfer(0xFF);					// enter shutdown mode
		SPI.transfer(0xFF);					// enter shutdown mode
		digitalWrite(_csPin, HIGH);
	#if TC77_DEBUG
		Serial.println("TC77 in shutdown mode");
	#endif
	}
	else if (shutdownMode && !mode) {
		shutdownMode = false;
		digitalWrite(_csPin, LOW);
		SPI.transfer(0x00);					// temperature byte
		SPI.transfer(0x00);					// temperature byte
		SPI.transfer(0x00);					// enter shutdown mode
		SPI.transfer(0x00);					// enter shutdown mode
		digitalWrite(_csPin, HIGH);
	#if TC77_DEBUG
		Serial.println("TC77 in conversion mode");
	#endif
	}
}