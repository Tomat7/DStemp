/*
  DStemp.cpp - Library to operate with DS18B20
  Created by Tomat7, October 2017, 2018, 2021.
  
  See updates on https://github.com/Tomat7/DStemp 
  Review branches test* for datails, comments and debug options
  
  The code of setCorrection() from http://blog.asifalam.com/ds18b20-change-resolution/ (c) Asif Alam
*/

#ifndef DStemp_h
#define DStemp_h

#include "Arduino.h"
#include <OneWire.h>

#ifdef DS_LIBVERSION
#undef DS_LIBVERSION
#endif
#define DS_LIBVERSION "DStemp_v20210404 on pin "

#define DS_CONV_TIMEOUT 750
#define HI_RESOLUTION 12

#define DS_T_MIN -55			// Minimal temperature by DataSheet
#define DS_T_ERR_OTHER -59 	// sensor was found but something going wrong during conversation (rare)
#define DS_T_ERR_CRC -71		// sensor was found but CRC error (often)
#define DS_T_ERR_TIMEOUT -82	// sensor was found but conversation not finished within defined timeout (may be)
#define DS_T_ERR_NOSENSOR -99	// Sensor Not Connected (not found)
#define DS_T_CORR 80			// What temperature start correction (if available on TH & TL)


class DSThermometer
{
public:

	DSThermometer(uint8_t pin);
    void init();
	void init(uint16_t convtimeout);

	void check();
	void control();
	void printConfig();
	void adjust();
	bool writeCorrection(byte _bits, int8_t _th, int8_t _tl);
	
	float Temp = DS_T_MIN;
	unsigned long dsMillis;
	uint16_t TimeConv = DS_CONV_TIMEOUT;
    bool Connected = false;
	bool Parasite = false;
	int8_t THreg = 0;
	int8_t TLreg = 0;
	int8_t CFGreg = 0;
	
	String LibConfig;

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
