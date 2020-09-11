#ifndef mqtt_utils_h
#define mqtt_utils_h

/*
 * Comment to block-out all MQTT calls, for debugging.
 */
#define MQTT_ENABLED

#include <Arduino.h>

struct mqtt_conf {
   char server[40] = "192.168.2.1";
   int port = 1883;
   char username[15] = "";
   char password[15] = "";
   char client_id[25] = "client_id";
   char base_topic[40] = "/home-iot/device";
   char log_topic[15] = "/log";
};

class MqttUtils {
   public:
      const struct mqtt_conf *conf;

      MqttUtils();

      void init(const struct mqtt_conf *conf);
      bool reconnect();
      void loop();

      /*
       * Publishes under topic = topic_base + subtopic".
       */
      void publish(const char *subtopic, const char *msg);
      void subscribe(const char *subtopic);
      void log_mqtt(const char *fmt, ...);

      static void subscribe_topics();
      static void callback(const char* topic, byte* payload_bytes, unsigned int size);

   private:
      bool initialized = false;
};

#endif

