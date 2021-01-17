/*
	DSThermometer.cpp - Library to operate with DS18B20
	Created by Tomat7. October 2017, 2018.
*/
/*
	check() returns NOTHING!
	only check for temperature changes from OneWire sensor
	
	-99  - sensor not found
	-82  - sensor was found but conversation not finished within defined timeout (may be)
	-71  - sensor was found but CRC error (often)
	-59  - sensor was found but something going wrong during conversation (rare)
	
	Connected == 0 значит датчика нет - no sensor found
	в dsMillis хранится millis() c момента запроса или крайнего Init()
	и от dsMillis отсчитывается msConvTimeout
*/

// See updates on https://github.com/Tomat7/DStemp 
// Review branches test* for datails, comments and debug options

#include "Arduino.h"
#include "DStemp.h"
#include <OneWire.h>

// Scratchpad locations. Thanks to Miles Burton <miles@mnetcs.com> for DallasTemperature library
#define TEMP_LSB        0
#define TEMP_MSB        1
#define HIGH_ALARM_TEMP 2
#define LOW_ALARM_TEMP  3
#define CONFIGURATION   4
#define INTERNAL_BYTE   5
#define COUNT_REMAIN    6
#define COUNT_PER_C     7
#define SCRATCHPAD_CRC  8

// OneWire commands
#define STARTCONVO      0x44  // Tells device to take a temperature reading and put it on the scratchpad
#define COPYSCRATCH     0x48  // Copy scratchpad to EEPROM
#define READSCRATCH     0xBE  // Read from scratchpad
#define WRITESCRATCH    0x4E  // Write to scratchpad
#define RECALLSCRATCH   0xB8  // Recall from EEPROM to scratchpad
#define READPOWERSUPPLY 0xB4  // Determine if device needs parasite power
#define ONLYONEDSDEVICE 0xCC  // Only ONE sensor on wire



DSThermometer::DSThermometer(uint8_t pin):ds(pin) 
{
	_pin = pin;
}

void DSThermometer::init() 
{
	init(DS_CONVERSATION_TIME);
	printConfig();
	setResolution(HI_RESOLUTION);
}

void DSThermometer::init(uint16_t convtimeout)
{
	_msConvTimeout = convtimeout;
	LibConfig = LIBVERSION + String(_pin);
	initOW();
}

void DSThermometer::control()
{
	check();
}
	
void DSThermometer::check()
{
	uint16_t msConvDuration = millis() - dsMillis;	
	
	if (Connected && (ds.read_bit() == 1))   	// вроде готов отдать данные
	{
		Temp = askOWtemp();  	// можем ещё получить -71 (CRC error) или -59 (other error)
		TimeConv = msConvDuration;
	}								
	else if (msConvDuration > _msConvTimeout)	// подключен, но время на преобразование истекло и не готов отдать данные
	{	
		Temp = T_ERR_TIMEOUT;	// датчик был, но оторвали на ходу или не успел - косяк короче: -82
	} 
	else return;
	
	if (Temp > T_MIN) requestOW();
	else initOW();
	
	return;
}

float DSThermometer::askOWtemp()
{
	
	byte present = 0;
	byte bufData[9]; // буфер данных
	float owTemp;
	
	present = ds.reset();
	if (present)
	{
		ds.write(ONLYONEDSDEVICE);
		ds.write(READSCRATCH);                     		// Read Scratchpad
		ds.read_bytes(bufData, 9);                 		// чтение памяти датчика, 9 байтов
		if (OneWire::crc8(bufData, 8) == bufData[SCRATCHPAD_CRC])	// проверка CRC
		{
#if defined(__AVR__)
			owTemp = (float) ((int) bufData[0] | (((int) bufData[1]) << 8)) * 0.0625; // ХЗ откуда стащил формулу
#elif defined(ESP32) || defined(ESP8266)
			int16_t rawTemp = (((int16_t) bufData[TEMP_MSB]) << 11) | (((int16_t) bufData[TEMP_LSB]) << 3);
			owTemp = (float) rawTemp * 0.0078125f;  // стащил из DallasTemperature
#endif
		} 
		else
		{
			owTemp = T_ERR_CRC;      // ошибка CRC, вернем -71
		}
	}
	else
	{
		owTemp = T_ERR_OTHER;       // наверное такой косяк тоже может быть, надо разбираться
	}
	return owTemp;
}

void DSThermometer::requestOW()
{
	ds.reset();
	ds.write(ONLYONEDSDEVICE);
	ds.write(STARTCONVO, Parasite);
	dsMillis = millis();
	return;
}

void DSThermometer::initOW()
{
	byte addr[8];
	ds.reset_search();
	Connected = (ds.search(addr));
	// --- check for Parasite Power
	ds.reset();	
	ds.write(ONLYONEDSDEVICE);
	ds.write(READPOWERSUPPLY);
	if (ds.read_bit() == 0) Parasite = true;
	else 					Parasite = false; 
	// --- END check for Parasite Power
	dsMillis = millis();
	if (Connected) requestOW();
	else Temp = T_ERR_NOSENSOR;
	return;
}

void DSThermometer::printConfig()
{
	Serial.println(LibConfig);
	//Serial.print(F(LIBVERSION));
	//Serial.println(_pin);
	return;
}

void DSThermometer::setResolution(byte resolution_bits)  // (c) Asif Alam's Blog http://blog.asifalam.com/ds18b20-change-resolution/
{
	byte reg_cmd;
	switch (resolution_bits) 
	{
		case 9: reg_cmd = 0x1F; break;
		case 10: reg_cmd = 0x3F; break;
		case 11: reg_cmd = 0x5F; break;
		case 12: reg_cmd = 0x7F; break;
		default: reg_cmd = 0x7F; break;
	}
	ds.reset();
	ds.write(ONLYONEDSDEVICE);  	// No address - only one DS on line
	ds.write(WRITESCRATCH);  	// Write scratchpad command
	ds.write(0);     	// TL data
	ds.write(0);     	// TH data
	ds.write(reg_cmd); 	// Configuration Register (resolution) 7F=12bits 5F=11bits 3F=10bits 1F=9bits
	ds.reset();      	// This "reset" sequence is mandatory
	ds.write(ONLYONEDSDEVICE);
	ds.write(COPYSCRATCH, Parasite);  	// Copy Scratchpad command
	delay(20); 			// added 20ms delay to allow 10ms long EEPROM write operation (DallasTemperature)
	if (Parasite) delay(10); 	// 10ms delay
	ds.reset();
	return;
}
