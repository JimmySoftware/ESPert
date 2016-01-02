# ESPert
Welcome to the ESPert Arduino library for beta testers of the ESPresso Lite.
Please proceed to follow the procedures as prescribed in the ESPresso Lite tutorial.

# Setup
## Arduino Software
- Install Arduino 1.6.7 from https://www.arduino.cc/en/Main/Software

## ESP8266 Board Manager (Staging Version)
- Start Arduino and open Preference
- Enter the URL http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. If you have multiple URLs, separating them with commas.
- Open Boards Manager from Tools > Board > Board Manager then install esp8266 by ESP8266 Community (current version is 2.0.0)
- From menu Tools > Board select Generic ESP8266 Module

## ESPert library
- Download or Clone ESPert library and examples from https://github.com/JimmySoftware/ESPert 
- Copy to Arduino Library directory (~/Document/Arduino/Libraries)

## Install dependency for ESPert
Open Library Manager (menu Sketch > Include Library > Manage Libraries…) then install the following libraries
- DHT sensor library by Adafruit (current version 1.2.3)
- PubSubClient by Nick O'Leary (current version 2.4.0)
- ArduinoJson by Benoit Blanchon (current version 5.0.7)
- HttpClient by Adrian McEwen (current version 2.2.0)
- ESP8266 Oled Driver for SSD1306 display by Daniel Eichborn (current version 2.0.0)
- Adafruit NeoPixel by Adafruit (current version 1.0.4)

## Run examples from ESPert
- Open Arduino IDE, try examples of ESPert library

