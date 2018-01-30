# DStemp
Another OneWire library extension for DS18B20 sensors. (one PIN - one DS)
 
***WARNING!***  **ONE pin - ONE sensor!**  
---

For multiply sensors on PIN use DallasTemperature library!
---
 
**Methods:**  

   **check()** returns NOTHING but does everything:
   основной метод, делает всё:
   - проверяет наличие датчика / check for sensor
   - инициализирует его при необходимости / reinit sensor if necessary
   - командует датчику начать преобразование температуры / start conversation
   - получает результат от датчика / collect results
   
   **Temp** return current temperature OR:  
   -99  sensor not found  
   -83  sensor was found but conversation not finished within defined timeout  
   -77  sensor was found and ready but something going wrong  
   -71  sensor was found but CRC error  

   **Connected** == 0 значит датчика нет - no sensor found
   в **dsMiilis** хранится millis() c момента запроса или крайнего **Init()**
   от **dsMillis** отсчитывается **msConvTimeout** - время потраченное на преобразование (DEBUG!)

If neсessary it also request temperature conversation and/or INIT sensor for 12-bit resolution.  
