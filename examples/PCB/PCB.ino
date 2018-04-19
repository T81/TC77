/*
 Use of TC77 library to read the PCB temperature
 
 This file is free software; you can redistribute it and/or modify
 it under the terms of either the GNU General Public License version 3
 published by the Free Software Foundation.
 */
 

#include <SPI.h>	// include the SPI library
#include <TC77.h>
TC77 tempPCB;		// create the TC77 temperature sensor object

void setup() {

  Serial.begin(9600);
  SPI.begin();                 				// wake up the SPI
  SPI.setDataMode(SPI_MODE3); 				// datasheet p6-7
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV32);  	// datasheet p6
  tempPCB.Begin(9);							// initialize the communication using pin 9 as cs
  Serial.println("");
  myTC.ID();  								// print the TC77 ID and halt in case of error

}

void loop() {
  if (myTC.Update()) {						// if new temperature measurement is available print raw data and temperature conversion
    Serial.print("TC77 RAW: ");
    Serial.print(myTC.readRaw());
    Serial.print("\t Degrees: ");
    Serial.println(myTC.Temp(),4);
  }
  delay(500);
}