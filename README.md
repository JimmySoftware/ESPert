# ESPert
Welcome to the ESPert Arduino library for beta testers of the ESPresso Lite.
Please proceed to follow the procedures as prescribed in the ESPresso Lite tutorial.

### Setup
# Arduino Software
- Install Arduino 1.6.6 from https://www.arduino.cc/en/Main/Software

# ESP8266 Board Manager (Staging Version)
- Start Arduino and open Preference
- Enter the URL http://arduino.esp8266.com/staging/package_esp8266com_index.json into Additional Board Manager URLs field. If you have multiple URLs, separating them with commas.
- Open Boards Manager from Tools > Board > Board Manager then install esp8266 by ESP8266 Community (current version is 2.0.0-rc2)
- From menu Tools > Board select Generic ESP8266 Module

# ESPert library
- Download or Clone ESPert library and examples from https://github.com/JimmySoftware/ESPert

# Install dependency for ESPert
Open Library Manager (menu Sketch > Include Library > Manage Libraries…) then install the following libraries
- Adafruit GFX library by Adafruit (current version 1.0.2)
- DHT sensor library by Adafruit (current version 1.2.3)
- ArduinoJson by Benoit Blanchon (current version 5.0.7)

# Run examples from ESPert
- Open Arduino IDE, try examples of ESPert library

