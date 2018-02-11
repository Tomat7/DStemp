/*
  Example for DStemp library on Arduino Nano with THREE sensors (c) Tomat7, Jan 2018

  Method check() returns NOTHING but does everything:
  основной метод, делает всё:
  - проверяет наличие датчика / check for sensor
  - инициализирует его при необходимости / reinit sensor if necessary
  - командует датчику начать преобразование температуры / start conversation
  - получает результат от датчика / collect results

  Temp return current temperature OR:
  -99  sensor not found
  -83  sensor was found but conversation not finished within defined timeout
  -77  sensor was found and ready but something going wrong
  -71  sensor was found but CRC error

  Connected == 0 значит датчика нет - no sensor found
  в dsMiilis хранится millis() c момента запроса или крайнего Init()
  от dsMillis отсчитывается TimeConv - время потраченное на преобразование (DEBUG!)
*/
#define SKETCHVERSION __FILE__ " " __DATE__ " " __TIME__
#define SERIALSPEED 115200
#define DS_CONVTIME 750

#include "DStemp.h"
DSThermometer sensor[] = { 5, 6, 7 };  // на эти пины подключается по **одному** датчику DS18B20
const int sensCount = sizeof(sensor) / sizeof(DSThermometer);
unsigned long msReinit;

void setup()
{
  Serial.begin(SERIALSPEED);
  delay(500);
  Serial.println(SKETCHVERSION);
  for (int i = 0; i < sensCount; i++)
  {
    sensor[i].init(DS_CONVTIME);
    String dsInfo = "ID " + String(i, DEC) + "|Connected " + String(sensor[i].Connected, DEC);
    Serial.println(dsInfo);
  }
  msReinit = millis();
}

void loop()
{
  for (int i = 0; i < sensCount; i++) sensor[i].check();

  if ((millis() - msReinit) > DS_CONVTIME)
  {
    for (int i = 0; i < sensCount; i++)
    {
      float t = sensor[i].Temp;   // для наглядности
      String dsInfo = "DS " + String(i, DEC) + ": " + String(t, 2) + " | parasite: " +
                      String(sensor[i].Parasite, DEC) + " | " + String(sensor[i].TimeConv, DEC);
      Serial.println(dsInfo);
    }
    msReinit = millis();
  }
}
