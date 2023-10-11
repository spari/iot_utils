#ifndef time_utils_h
#define time_utils_h

#include <Arduino.h>

struct time_conf {
   char timezone[15] = "UTC-05:30";
   int daylight_savings = 0;
   char ntp_server[40] = "pool.ntp.org";
};

class TimeUtils {
   public:
      const struct time_conf *conf;

      TimeUtils();

      void init(const struct time_conf *conf, std::function<void()> retry_callback = nullptr, int retries=60);
      void reconnect(uint8_t retries);
      String ctime_str();
      String uptime_str();
      void setRetryCallback(std::function<void()> retry_callback);

   private:
      bool connect();
      void set_local_time(time_t time0);
      std::function<void()> retry_callback;
};

#endif
