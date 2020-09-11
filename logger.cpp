#include "logger.h" 

#include <cstdarg>
#include <cstdio>
#include <Arduino.h>

/*
 * This prints to serial output.
 * Not called directly, but via macros defined in header.
 */
void log_proxy(const char *fmt, ...)
{
   va_list argptr;
   va_start(argptr, fmt);

   vprintf(fmt, argptr);

   va_end(argptr);
}

/*
 * Retain even though same as above.
 * log_proxy() by default logs to serial port. But log_proxy() 
 * may get macro-replaced by log_mqtt() if MQTT is enabled.
 * Explicit calls to log_serial() do not get replaced.  
 */
void log_serial(const char *fmt, ...)
{
   va_list argptr;
   va_start(argptr, fmt);

   vprintf(fmt, argptr);

   va_end(argptr);
}


