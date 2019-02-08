/*
  DSThermometer.cpp - Library to operate with DS18B20
  Created by Tomat7, October 2017, 2018.
  
  See updates on https://github.com/Tomat7/DStemp 
  Review branches test* for datails, comments and debug options
  
  The code of setResolution() from http://blog.asifalam.com/ds18b20-change-resolution/ (c) Asif Alam
*/

#ifndef DStemp_h
#define DStemp_h

#include "Arduino.h"
#include <OneWire.h>

#ifdef LIBVERSION
#undef LIBVERSION
#endif
#define LIBVERSION "DStemp_v20181119 on pin "


#define DS_CONVERSATION_TIME 750
#define HI_RESOLUTION 12

#define T_MIN -55			// Minimal temperature by DataSheet
#define T_ERR_OTHER -59 	// sensor was found but something going wrong during conversation (rare)
#define T_ERR_CRC -71		// sensor was found but CRC error (often)
#define T_ERR_TIMEOUT -82	// sensor was found but conversation not finished within defined timeout (may be)
#define T_ERR_NOSENSOR -99	// Sensor Not Connected (not found)


class DSThermometer
{
public:

	DSThermometer(uint8_t pin);
    void init();
	void init(uint16_t convtimeout);

	void check();
	void control();
	void printConfig();
	void setResolution(byte res_bit);
	
	float Temp;
	unsigned long dsMillis;
	uint16_t TimeConv;
    bool Connected;   // в нем хранится millis() c момента запроса или попытки инициализации
	bool Parasite;

private:
    //OneWire *_ds;
	OneWire ds;
	byte _pin;
    uint16_t _msConvTimeout;
    float askOWtemp();
	void requestOW();
    void initOW();
};
#endif
