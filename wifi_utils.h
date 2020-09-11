#ifndef wifi_utils_h
#define wifi_utils_h

/*
 * These includes need to remain here, as it looks like they need to get 
 * included in the calling program, even if not directly used there.
 */
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#include <Arduino.h>

struct wifi_conf {
   char ssid[20];
   char password[20];
};

class WifiUtils {
   public: 
      const struct wifi_conf *conf;

      WifiUtils();

      void init(const struct wifi_conf *conf);
      void connect();
      String get_mac();
      String get_ip();

   private:
      void setup_sta_client();
};

#endif
