
#include <ESPert.h>

ESPert espert;

void setup() {
  espert.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  espert.LED.on();
  espert.println( "LED on" );
  delay( 1000 );

  espert.LED.off();
  espert.println( "LED off" );
  delay( 1000 );
}


