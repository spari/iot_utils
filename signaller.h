#ifndef signaller_h 
#define signaller_h 

#include <Arduino.h>

/*
 * If LED is not used, caller should set it to BUILTIN_LED.
 * IF buzzer is not used, caller should set it to -1.
 */
struct signaller_gpio {
   uint8_t sysok_led;  // System ok.
   uint8_t status_led; // Status Codes and Notifications.
   uint8_t buzzer;     // Set to -1 if none.
};

class Signaller {
   public: 
      static struct signaller_gpio gpio;

      Signaller();

      void init();
      void smoke_signal(uint8_t num);
      void alert(uint8_t nblinks); 
      void notify(int num_secs);
      void ok();

   private:
      /*
       * Set accordingly depending on if you're using the pins as sink or 
       * source.
       */
      const boolean LED_OFF = HIGH;
      const boolean LED_ON = LOW;
      const boolean BUZZER_OFF = HIGH;
      const boolean BUZZER_ON = LOW;
};

#endif
