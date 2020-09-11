#include "ota_utils.h"

#include <Arduino.h>
#include <ArduinoOTA.h>

OtaUtils::OtaUtils() {}

void OtaUtils::init()
{
   Serial.println("Intializing OtaUtils.");

   ArduinoOTA.onStart([]() {
      Serial.println("Update started.");
   });

   ArduinoOTA.onEnd([]() {
      Serial.println("Update completed.");
      Serial.println("Resetting device.");
      delay(1000);
   });

   ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      //log_serial("Progress: %u%%\r", uint8_t(progress / (total / 100)));
   });

   ArduinoOTA.onError([](ota_error_t error) {
      Serial.println("Error: " + error);
      if (error == OTA_AUTH_ERROR) {
         Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
         Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
         Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
         Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
         Serial.println("End Failed");
      }
   });

   ArduinoOTA.begin();
}

void OtaUtils::loop()
{
   ArduinoOTA.handle();
}
