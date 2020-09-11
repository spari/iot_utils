#include "wifi_utils.h"

#include <logger.h>

#include <Arduino.h>

WiFiClient client;

WifiUtils::WifiUtils() {}

void WifiUtils::init(const struct wifi_conf *conf)
{ 
   Serial.println("Intializing WifiUtils.");

   this->conf = conf;

   connect();
}

void WifiUtils::connect()
{
   WiFi.mode(WIFI_STA);

   setup_sta_client();
}

void WifiUtils::setup_sta_client()
{
   Serial.print("  Connecting to WiFi SSID '" + String(conf->ssid) + "'.");

   WiFi.begin(conf->ssid, conf->password);

   // Need to wait few seconds before checking status.
   delay(1000);

   while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(500);
   }

   Serial.println(" Wifi connected.");
   Serial.println("  IP Address: " + get_ip());
}

String WifiUtils::get_mac()
{
   unsigned char mac_addr[6];

   WiFi.macAddress(mac_addr);

   char mac_str[18];
   snprintf(mac_str, sizeof(mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
      mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

   return String(mac_str);
}

String WifiUtils::get_ip()
{
   return WiFi.localIP().toString();
}

