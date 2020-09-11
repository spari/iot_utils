#include "signaller.h"

/*
 * Use Alarm.delay() instead of delay() so that Alarms are not affected by
 * delay().
 */
#include <TimeAlarms.h>
#define delay(millis) Alarm.delay(millis)

Signaller::Signaller() {}

void Signaller::init()
{
    Serial.println("Intializing Signaller.");

   pinMode(gpio.sysok_led, OUTPUT);
   digitalWrite(gpio.sysok_led, LED_OFF);

   pinMode(gpio.status_led, OUTPUT);
   digitalWrite(gpio.status_led, LED_OFF);

   if (gpio.buzzer != -1) {
      pinMode(gpio.buzzer, OUTPUT);
      digitalWrite(gpio.buzzer, BUZZER_OFF);
   }
}

/*
 * Blinks out 4-digit binary code of the number provided.
 *
 *    Binary 1 = long blink (dash)
 *    Binary 0 = short blink (dot)
 *
 * Example: Decimal 9 = binary 1001 = dash-dot-dot-dash
 *
 * Each nibble is separated by a one second pause.
 * This function lasts total of 4 seconds.
 *
 * Used to blink error/status code for field diagnostics.
 */
void Signaller::smoke_signal(uint8_t num)
{
   if (num > 15) return;

   for (int bits=3; bits>-1; bits--) {
      if (num & (1<<bits)) {
         digitalWrite(gpio.status_led, LED_ON);
         delay(500);
         digitalWrite(gpio.status_led, LED_OFF);
         delay(500);
      }
      else {
         digitalWrite(gpio.status_led, LED_ON);
         delay(50);
         digitalWrite(gpio.status_led, LED_OFF);
         delay(450);
      }
   }
   delay(1000);
}

/*
 * This function lasts approximately num_secs seconds.
 */
void Signaller::warn(uint8_t code, int num_secs)
{
   if (num_secs < 4) num_secs = 4;

   // Scale-down num_secs by 4, as smoke_signal() lasts 4 seconds.
   for (int i=0; i<num_secs/4; i++) {
      smoke_signal(code);
   }
}

/*
 * This function lasts 1 second.
 */
void Signaller::ok()
{
   //noInterrupts();
   digitalWrite(gpio.sysok_led, LED_ON);
   delay(100);
   digitalWrite(gpio.sysok_led, LED_OFF);
   delay(900);
   //interrupts();
}

/*
 * This function lasts num_sec seconds.
 */
void Signaller::notify(int num_secs)
{
   const int blinks_per_sec = 4;

   for (int i=0; i<num_secs; i++) {
      for (int j=0; j<blinks_per_sec; j++) {
         if (gpio.buzzer != -1) digitalWrite(gpio.buzzer, BUZZER_ON);
         digitalWrite(gpio.status_led, LED_ON);
         delay(50);
         digitalWrite(gpio.status_led, LED_OFF);
         if (gpio.buzzer != -1) digitalWrite(gpio.buzzer, BUZZER_OFF);
         delay(75);
      }
      delay(500);
   }
}

