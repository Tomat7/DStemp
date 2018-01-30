# DStemp
Another OneWire library extension for DS18B20 sensors. (one PIN - one DS)
 
***WARNING!***  **ONE pin - ONE sensor!**  
---

For multiply sensors on PIN use DallasTemperature library!
---
 
**Methods:**  
 
_getTemp()_ - check, update and returns temperature from OneWire sensor.  
If neсessary it also request temperature conversation and/or INIT sensor for 12-bit resolution.  
 
_getTemp()_ may return:  
 
  -99 - sensor not found  
  -55 - sensor was found but conversation not finished within defined timeout  
  -22 - sensor was found but CRC error  
  -11 - sensor was found but something going wrong  
   
 _Status_ - keep Millis() of last request_for_conversation or 0 if sensor not found.  
