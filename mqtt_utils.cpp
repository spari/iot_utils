#include "mqtt_utils.h"

#include <WiFiClient.h>
#include <PubSubClient.h>

#ifdef MQTT_ENABLED

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

MqttUtils::MqttUtils() {}

void MqttUtils::init(const struct mqtt_conf *conf)
{
   Serial.println("Intializing MqttUtils.");

   this->conf = conf;

   mqtt_client.setServer(conf->server, conf->port);
   mqtt_client.setCallback(callback);

   reconnect();

   initialized = true;
}

void MqttUtils::loop()
{
   mqtt_client.loop();
}

bool MqttUtils::reconnect()
{
   if (mqtt_client.connected()) {
     return true;
   }

   Serial.print("  Connecting to MQTT server '" + String(conf->server) + "'.");

#ifdef DEBUG_LOG
   log_serialln("------ MQTT -----\r\n");
   log_serialln("server:            '%s'", conf->server);
   log_serialln("port:              '%d'", conf->port);
   log_serialln("username:          '%s'", conf->username);
   log_serialln("password:          '%s'", conf->password);
   log_serialln("client_id:         '%s'", conf->client_id);
   log_serialln("base_topic:        '%s'", conf->base_topic);
   log_serialln("log_topic:         '%s'", conf->log_topic);
   log_serialln("-----------------\r\n");
#endif

   mqtt_client.connect(conf->client_id, conf->username, conf->password);

   if (mqtt_client.connected()) {
      Serial.println(" MQTT connected.");
      subscribe_topics();
      return true;
   }
   else {
      Serial.println(" MQTT connection failed.");
      return false;
   }
}


const int MQTT_TOPIC_SIZE = 80;

void MqttUtils::publish(const char *subtopic, const char *msg)
{
   char topic[MQTT_TOPIC_SIZE];

   reconnect();

   strncpy(topic, conf->base_topic, sizeof(topic));
   strncat(topic, subtopic, sizeof(topic));

   mqtt_client.publish(topic, msg);
}

void MqttUtils::subscribe(const char *subtopic)
{
   char topic[MQTT_TOPIC_SIZE];

   strncpy(topic, conf->base_topic, sizeof(topic));
   strncat(topic, subtopic, sizeof(topic));

   mqtt_client.subscribe(topic);
}

/*
 * If mqtt is connected logs to MQTT, else logs to serial. 
 */
void MqttUtils::log_mqtt(const char *fmt, ...)
{
   const int MQTT_BUFF_SIZE = 150;
   
   va_list args;
   va_start(args, fmt);

   if (reconnect()) {
      char buff[MQTT_BUFF_SIZE];
      vsnprintf(buff, MQTT_BUFF_SIZE, fmt, args);

      if (initialized) {
         // Log to MQTT.
         publish(conf->log_topic, buff);
      }
   }
   else {
      // Log to serial.
      String str = String(fmt);
      if (str.endsWith("\n")) {
         str.replace("\n", "\r\n");
      } 
      else {
         str.concat("\r\n"); 
      }
      vprintf(str.c_str(), args);
   }

   va_end(args);
}

#else

MqttUtils::MqttUtils() {}
void MqttUtils::init() {}
void MqttUtils::loop() {}
void MqttUtils::reconnect() {}
void MqttUtils::publish(const char *topic, char *msg) {}
void MqttUtils::subscribe(const char *topic) {}
void MqttUtils::log_mqtt(const char *fmt, ...) {}

#endif
