// здесь все функции
/*
DSThermometer.cpp - Library to operate with DS18B20
Created by Tomat7, October 2017.
*/
#include "Arduino.h"
#include "DStemp.h"
#include <OneWire.h>


DSThermometer::DSThermometer(uint8_t pin):ds(pin) 
{
	_pin = pin;
}

void DSThermometer::init()
{
	init(DS_CONVERSATION_TIME, true, true);
}

void DSThermometer::init(uint16_t convtimeout)
{
	init(convtimeout, true, true);
}

void DSThermometer::init(uint16_t convtimeout, bool printConfig)
{
	init(convtimeout, printConfig, false);
}

void DSThermometer::init(uint16_t convtimeout, bool printConfig, bool setHiRes)
{
	//	Serial.print("init.. ");
	_msConvTimeout = convtimeout;
	initOW();
	//setHiResolution();
	if (printConfig)
	{
		Serial.print(F(LIBVERSION));
		Serial.println(_pin);
	}
	if (setHiRes) setHiResolution();
#ifdef DEBUG2
	Serial.print(_temperature);
	Serial.print(" -init-stop..");
	Serial.println(millis());
#endif
}

/*
check() returns NOTHING!
only check for temperature changes from OneWire sensor
*** NOT IN THIS VERSION!! -100 - conversation not finished yet but sensor still OK
-99  - sensor not found
-82  - sensor was found but conversation not finished within defined timeout (may be)
-71  - sensor was found but CRC error (often)
-59  - sensor was found but something going wrong during conversation (rare)

Connected == 0 значит датчика нет - no sensor found
в dsMillis хранится millis() c момента запроса или крайнего Init()
и от dsMillis отсчитывается msConvTimeout
*/


void DSThermometer::check()
{
#define TIMEISOUT ((millis() - dsMillis) > _msConvTimeout)

	if (Connected && (ds.read_bit() == 1))   // вроде готов отдать данные
	{
		//Serial.print("+");          
		Temp = askOWtemp();  	// но можем ещё получить -71 или -59
		TimeConv = millis() - dsMillis;
	}								
	else if TIMEISOUT  			// подключен, но время на преобразование истекло и не готов отдать данные
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
		ds.write(0xCC);
		ds.write(0xBE);                            // Read Scratchpad
		ds.read_bytes(bufData, 9);                 // чтение памяти датчика, 9 байтов
		if (OneWire::crc8(bufData, 8) == bufData[8])  // проверка CRC
		{
			//Serial.print("+");      // типа всё хорошо!
			owTemp = (float) ((int) bufData[0] | (((int) bufData[1]) << 8)) * 0.0625; // ХЗ откуда стащил формулу
		} 
		else
		{
			//Serial.print("*");
			owTemp = T_ERR_CRC;           // ошибка CRC, вернем -71
		}
	}
	else
	{
		//Serial.print("-");        // датчик есть и готов, но не отдал температуру, вернем -59,
		owTemp = T_ERR_OTHER;             // короче, наверное такой косяк тоже может быть, надо разбираться
	}
	return owTemp;
}

void DSThermometer::requestOW()
{
	ds.reset();
	ds.write(0xCC);
	ds.write(0x44, Parasite);
	dsMillis = millis();
#ifdef DEBUG3
	Serial.print("reqOW-stop-");
	Serial.println(millis());
#endif
	return;
}

void DSThermometer::initOW()
{
	byte addr[8];
	ds.reset_search();
	Connected = (ds.search(addr));
	// --- check for Parasite Power
	ds.reset();	
	ds.write(0xCC);
	ds.write(0xB4);
	if (ds.read_bit() == 0) Parasite = true;
	else 					Parasite = false; 
	// --- END check for Parasite Power
	//ds.reset_search();
	dsMillis = millis();
	if (Connected) requestOW();
	else Temp = T_ERR_NOSENSOR;
#ifdef DEBUG4
	Serial.print("initOW-stop-");
	Serial.println(millis());
#endif
	return;
}

void DSThermometer::setHiResolution()
{
#ifdef DS_SET_HI_RESOLUTION
	// --- Setup 12-bit resolution
	ds.reset();
	ds.write(0xCC);  // No address - only one DS on line
	ds.write(0x4E);  // Write scratchpad command
	ds.write(0);     // TL data
	ds.write(0);     // TH data
	ds.write(0x7F); // Configuration Register (resolution) 7F=12bits 5F=11bits 3F=10bits 1F=9bits
	ds.reset();      // This "reset" sequence is mandatory
	ds.write(0xCC);
	ds.write(0x48, Parasite);  // Copy Scratchpad command
	delay(20); 	// added 20ms delay to allow 10ms long EEPROM write operation (DallasTemperature)
	if (Parasite) delay(10); // 10ms delay
	ds.reset();
#endif
}


