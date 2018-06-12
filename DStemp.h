/*
  DSThermometer.cpp - Library to operate with DS18B20
  Created by Tomat7, October 2017.
  Released into the public domain.
*/
#ifndef DStemp_h
#define DStemp_h

#include "Arduino.h"
#include <OneWire.h>

#define LIBVERSION "DStemp_v20180531 on pin "
#define DS_CONVERSATION_TIME 750
#define INITATTEMPT 2

#define T_MIN -55				// Minimal temperature by DataSheet
#define T_ERR_CRC -71
#define T_ERR_OTHER -82 
#define T_ERR_TIMEOUT -93
#define T_ERR_FIRST -94			// первая поппытка найти пропавший датчик
#define T_ERR_SECOND -95		// вторая и финальная попытка
#define T_ERR_NOTCONNECTED -99	// Not Connected

/* !! OLD data !!
#define T_MIN 0				// Minimal temperature by DataSheet
#define T_ERR_CRC -11
#define T_ERR_OTHER -22 
#define T_ERR_TIMEOUT -33
#define T_ERR_FIRST -44
#define T_ERR_SECOND -55
#define T_ERR_NOTCONNECTED -99		// Not Connected
*/

//#define DSDEBUG true
//#define DEBUG2
//#define DEBUG3
//#define DEBUG4

class DSThermometer
{
public:
  // переменные - члены класса »нициализируютс¤ `при запуске
  // конструктор создает экземпляр Thermometer и инициализирует 
  // переменные-члены класса и состояние
    //DSThermometer(OneWire *ds);
	DSThermometer(uint8_t pin);
    void init();
	void init(uint16_t convtimeout);
	void init(uint16_t convtimeout, bool printConfig);
    void check();
	float Temp;
	unsigned long dsMillis;
	uint16_t TimeConv;
    bool Connected;   // в нем хранится millis() c момента запроса или попытки инициализации
	bool Parasite;
	//String LibVersion;
	//bool dsDebug;		// если время преобразование не итекло, то выдавать -100 
						// иначе выдавать температуру крайнего измерения
private:
    //OneWire *_ds;
	OneWire ds;
	byte _pin;
    float _temperature;
    uint16_t _msConvTimeout;
    float askOWtemp();
	void requestOW();
    void initOW();
	void setHiResolution();
};
#endif
