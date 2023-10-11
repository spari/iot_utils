#include "time_utils.h"
#include "logger.h" 

#include <Arduino.h>
#include <Time.h>
#include <PubSubClient.h>

#include <logger.h>

TimeUtils::TimeUtils() {}

static bool local_time_set = false;

void TimeUtils::init(const struct time_conf *conf, std::function<void()> retry_callback, int retries)
{
   Serial.println("Intializing TimeUtils.");

   this->conf = conf;
   this->retry_callback = retry_callback;

   //configTime(0, 0, conf->ntp_server, "pool.ntp.org");
   configTime(0, 0, conf->ntp_server);
   setenv("TZ", conf->timezone, 1);

   reconnect(retries);
}

void TimeUtils::setRetryCallback(std::function<void()> retry_callback) {
  this->retry_callback = retry_callback;
}

/*
 * Note: If fails to obtain time, then retry again in loop() or a timer 
 * callback instead of waiting indefinitely during boot. Use smaller 
 * retries in timer callback so that it is non-blocking. 
 */
void TimeUtils::reconnect(uint8_t retries) 
{
   time_t time0;
   extern PubSubClient mqtt_client;

   if (!local_time_set) {
      Serial.print("  Connecting to NTP server '"+ String(conf->ntp_server) + "'.");
      for (int i=0; i<retries; i++) {
         if (retry_callback) retry_callback();
         if (connect()) {
            Serial.println(" NTP connected.");
            return;
         }
         delay(1000);
      }
      Serial.println(" NTP connection failed.");
   }
}

/*
 * Note: time() Returns number of seconds elapsed since 
 * 00:00 hours, Jan 1, 1900 UTC.
 *
 * Testing for a "!now" doesn't seem to work.
 * So at least make sure year is > 2000.
 * Epoch time 947000000 is roughly year 2000.
 */
bool TimeUtils::connect() 
{
   time_t time0;

   /*
    * If time is already set, don't unecessarily retry.
    * It will have a chance to synchronize with NTP upon startup,
    * given the occassional power cuts at least once every few months.
    */
   if (local_time_set) return true; 

   if ((time0 = time(nullptr)) > 947000000) {
      set_local_time(time0); 
      return true;
   }
   else {
      Serial.print(".");
      return false;
   }
}

/*
 * Set Local Time.
 */
void TimeUtils::set_local_time(time_t time0)
{
   struct tm tm0 = *(localtime(&time0));
   setTime(tm0.tm_hour, tm0.tm_min, tm0.tm_sec, tm0.tm_mday, tm0.tm_mon+1, tm0.tm_year+1900);
   local_time_set = true;
}


String TimeUtils::ctime_str()
{
   char ctime_str0[80];

   time_t ctime = now();

   sprintf(ctime_str0, "%4d-%02d-%02d %02d:%02d:%02d",
      year(ctime), month(ctime), day(ctime),
      hour(ctime), minute(ctime), second(ctime));

   return String(ctime_str0);
}

/*
 * Credit to Evandro ("evandro_net", Arduino Forum) for this efficient
 * method for uptime(). https://forum.arduino.cc/index.php?topic=71212.0
 *
 * Note: Arduino millis() resets itself after every 2^32 ms = 49 days.
 */
String TimeUtils::uptime_str()
{
   unsigned long milli = millis();

   unsigned long secs = milli/1000;
   unsigned long mins = secs/60;
   unsigned int hours = mins/60;
   unsigned int days = hours/24;

   milli -= secs*1000;
   secs -= mins*60;
   mins -= hours*60;
   hours -= days*24;

   char uptime_str[20];
   sprintf(uptime_str, "%d days %02dh:%02dm:%02ds", (byte)days, (byte)hours, (byte)mins, (byte)secs);

   return String(uptime_str);
}
