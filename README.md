# DStemp
Another OneWire library extension for DS18B20 sensors. (one PIN - one DS)
 
***WARNING!***  **ONE pin - ONE sensor!**  
---

For multiply sensors on PIN use DallasTemperature library!
---
 
**Methods:**  

   **_check()_** returns NOTHING but does everything:
   основной метод, делает всё:
   - проверяет наличие датчика / check for sensor
   - инициализирует его при необходимости / reinit sensor if necessary
   - командует датчику начать преобразование температуры / start conversation
   - получает результат от датчика / collect results
   
   **_Temp_** return current temperature OR:  
   **-99**  sensor not found  
   **-83**  (-33) sensor was found but conversation not finished within defined timeout  
   **-77**  (-22) sensor was found and ready but something going wrong  
   **-71**  (-11) sensor was found but CRC error  

   **_Connected_** == 0 значит датчика нет - no sensor found  
   в **_dsMillis_** хранится millis() c момента запроса или крайней попытки инициализации  
   от **_dsMillis_** отсчитывается **_TimeConv_** - время потраченное на преобразование (DEBUG!)  
