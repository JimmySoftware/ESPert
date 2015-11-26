
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESPert.h>

#include <SoftwareSerial.h>
// rx tx buffer
SoftwareSerial swSer(12, 14, 128);
String readString;

void setup() {
  Serial.begin(115200);
  swSer.begin(9600);

  Serial.println("\nSoftware serial test started");

  for (char ch = ' '; ch <= 'z'; ch++) {
    swSer.write(ch);
  }
  swSer.println("");

  readString = "";
}

bool readSerial()
{
  while (swSer.available()) {
    char c = swSer.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }
  if( readString.length() > 0 )
    return true;
  return false;
}

void loop() {
  
  if( readSerial() ) {
    Serial.println( readString );
    readString = "";
  }
  if (Serial.available()) {
    swSer.write(Serial.read());
  }

}
