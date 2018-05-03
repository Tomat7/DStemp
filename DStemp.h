/*
  DSThermometer.cpp - Library to operate with DS18B20
  Created by Tomat7, October 2017.
  Released into the public domain.
*/
#ifndef DStemp_h
#define DStemp_h

#include "Arduino.h"
#include <OneWire.h>

#define LIBVERSION "DStemp_v20180121 on pin "
#define CONVERSATIONTIME 1000
#define INITATTEMPT 2

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
    void init(uint16_t convtimeout);
    void check();
	float Temp;
	unsigned long dsMillis;
	uint16_t TimeConv;
    bool Connected;   // в нем хранится millis() c момента запроса или попытки инициализации
	bool Parasite;
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
