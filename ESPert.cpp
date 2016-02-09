#include "ESPert.h"

static ESPert* _espert = NULL;

ESPert::ESPert() {
  _espert = this;
}

bool ESPert::checkFlashSize() {
  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();

  Serial.println();
  Serial.printf("Flash real id....: %08X\n", ESP.getFlashChipId());
  Serial.printf("Flash real size..: %u\n\n", realSize);

  Serial.printf("Flash ide size...: %u\n", ideSize);
  Serial.printf("Flash ide speed..: %u\n", ESP.getFlashChipSpeed());
  Serial.printf("Flash ide mode...: %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

  if (ideSize != realSize) {
    Serial.println("************ ERROR ************");
    Serial.println("Flash Chip configuration wrong!");
    Serial.println("*******************************\n");
    return false;
  } else {
    Serial.println("Flash Chip configuration ok.\n");
    return true;
  }
}

void ESPert::init(int type, long baud) {
  if (type != -1) {
    ESPertBoardType = type;
  }

  if (ESPertBoardType == ESPERT_BOARD_ESPRESSO_LITE2) {
    ESPERT_PIN_LED = 2;
    ESPERT_PIN_BUTTON = 13;
  }

  Serial.begin(baud);
  EEPROM.begin(512);

  wdt_disable();
  wdt_enable(WDTO_8S);

  led.init();
  button.init();

  delay(1500);

  checkFlashSize();
}

void ESPert::loop() {
  if (button.isLongPress()) {
    button.resetPressTime();
    println("ESPert: Long pressed!");
    int mode = wifi.getMode();

    if (mode == ESPERT_WIFI_MODE_CONNECT) {
      wifi.setAutoConnect(false);
    } else if (mode == ESPERT_WIFI_MODE_DISCONNECT) {
      _espert->eeprom.write(237, "ESPert:SmartConfig");
      _espert->println("ESPert: WiFi SmartConfig on restart!");

      delay(100);
      ESP.reset();
    }
  }

  oled.update();
}

#if ARDUINO >= 100
size_t ESPert::write(uint8_t c) {
#else
void ESPert::write(uint8_t c) {
#endif
  if (c == '\n') {
    Serial.println();
  } else if (c == '\r') {
    // skip em
  } else {
    Serial.print((char)c);
  }
#if ARDUINO >= 100
  return 1;
#endif
}

String ESPert::macToString(const uint8_t* mac) {
  String result = "";

  for (int i = 0; i < 6; i++) {
    result += String(mac[i], 16);

    if (i < 5) {
      result += ":";
    }
  }

  return result;
}

// ****************************************
// Device Info class
// ****************************************
String ESPert_Info::getId() {
  char textID[16] = {'\0'};
  sprintf(textID, "ESPert-%lu", ESP.getChipId());
  return String(textID);
}

float ESPert_Info::getLibraryVersion() {
  return ESPERT_LIBRARY_VERSION;
}

uint32_t ESPert_Info::getFreeHeap() {
  return ESP.getFreeHeap();
}

uint32_t ESPert_Info::getChipId() {
  return ESP.getChipId();
}

uint32_t ESPert_Info::getFlashChipId() {
  return ESP.getFlashChipId();
}

String ESPert_Info::getFlashChipInfo() {
  int flashChipID = ESP.getFlashChipId();
  int arraySize = (sizeof(ESPertFlashID) / sizeof(*ESPertFlashID));

  for (int i = 0; i < arraySize; i++) {
    if (ESP.getFlashChipId() == ESPertFlashID[i]) {
      return ESPertFlashDesc[i];
    }
  }

  return "Unknown Flash Chip";
}

uint32_t ESPert_Info::getFlashChipRealSize() {
  return ESP.getFlashChipRealSize();
}

uint32_t ESPert_Info::getFlashChipSize() {
  return ESP.getFlashChipSize();
}

uint32_t ESPert_Info::getFlashChipSpeed() {
  return ESP.getFlashChipSpeed();
}

uint32_t ESPert_Info::getFlashChipSizeByChipId() {
  return ESP.getFlashChipSizeByChipId();
}

// ****************************************
// SoftwareSerial class
// ****************************************
ESPert_SoftwareSerial::ESPert_SoftwareSerial() {
  swSerial = NULL;
}

void ESPert_SoftwareSerial::init(int rx, int tx, int buffer) {
  if (swSerial) {
    delete swSerial;
    swSerial = NULL;
  }

  swSerial = new SoftwareSerial(rx, tx, buffer);
}

String ESPert_SoftwareSerial::readString() {
  ESPertReadString = "";

  while (swSerial && swSerial->available()) {
    char c = swSerial->read();  //gets one byte from serial buffer
    ESPertReadString += c; //makes the string ESPertReadString
    delay(5);  //slow looping to allow buffer to fill with next character
  }

  if (ESPertReadString.length() > 0) {
    return ESPertReadString;
  }

  return String("");
}

size_t ESPert_SoftwareSerial::write(uint8_t b) {
  swSerial->write(b);
}

int ESPert_SoftwareSerial::read() {
  return swSerial->read();
}

int ESPert_SoftwareSerial::available() {
  return swSerial->available();
}

void ESPert_SoftwareSerial::flush() {
  swSerial->flush();
}

int ESPert_SoftwareSerial::peek() {
  swSerial->peek();
}

void ESPert_SoftwareSerial::begin(int baud) {
  swSerial->begin(baud);

  for (char ch = ' '; ch <= 'z'; ch++) {
    swSerial->write(ch);
  }

  swSerial->println("");
  ESPertReadString = "";
}

// ****************************************
// BLE class
// ****************************************
ESPert_BLE::ESPert_BLE() {
  swSerial = NULL;
}

bool ESPert_BLE::init(ESPert_SoftwareSerial* swSer) {
  swSerial = swSer;

  if (swSerial) {
    swSerial->write("AT");
    delay(100);

    ESPertReadString = swSerial->readString();
    _espert->println("ESPert: " + ESPertReadString);

    if (ESPertReadString == "OK") {
      delay(100);
      return true;
    }
  }

  return false;
}

String ESPert_BLE::getFirmwareVersion() {
  if (swSerial) {
    swSerial->write("AT+VERR?");
    delay(100);
    ESPertReadString = swSerial->readString();

    if (ESPertReadString.length() != 0) {
      return ESPertReadString.substring(7);
    }
  }

  return "Unknown";
}

bool ESPert_BLE::isOn() {
  if (swSerial) {
    swSerial->write("AT+IBEA?");
    delay(100);
    ESPertReadString = swSerial->readString();

    if (ESPertReadString.length() != 0) {
      _espert->println("ESPert: " + ESPertReadString);
      if (ESPertReadString == "OK+Get:1") {
        delay(100);
        return true;
      } else if (ESPertReadString == "OK+Get:0") {
        delay(100);
        return false;
      }
    }
  }

  _espert->println("ESPert: Unknown command!");
  return false;
}

bool ESPert_BLE::isOff() {
  return !isOn();
}

bool ESPert_BLE::on() {
  if (swSerial) {
    swSerial->write("AT+IBEA1");
    delay(100);
    ESPertReadString = swSerial->readString();

    if (ESPertReadString.length() != 0) {
      _espert->println("ESPert: " + ESPertReadString);

      if (ESPertReadString == "OK+Set:1") {
        delay(100);
        return true;
      } else if (ESPertReadString == "OK+Set:0") {
        delay(100);
        return false;
      }
    }
  }

  _espert->println("ESPert: Unknown command!");
  return false;
}

bool ESPert_BLE::off() {
  if (swSerial) {
    swSerial->write("AT+IBEA0");
    delay(100);
    ESPertReadString = swSerial->readString();

    if (ESPertReadString.length() != 0) {
      _espert->println("ESPert: " + ESPertReadString);

      if (ESPertReadString == "OK+Set:0") {
        delay(100);
        return true;
      } else if (ESPertReadString == "OK+Set:1") {
        delay(100);
        return false;
      }
    }
  }

  _espert->println("ESPert: Unknown command!");
  return false;
}

String ESPert_BLE::getUUID()
{
  String UUID = "";

  if (swSerial) {
    swSerial->write("AT+IBE0?");
    delay(100);
    ESPertReadString = swSerial->readString();

    if (ESPertReadString.length() != 0) {
      _espert->println("ESPert: " + ESPertReadString.substring(9));
      UUID += ESPertReadString.substring(9);
    }

    swSerial->write("AT+IBE1?");
    delay(100);
    ESPertReadString = swSerial->readString();

    if (ESPertReadString.length() != 0) {
      _espert->println("ESPert: " + ESPertReadString.substring(9));
      UUID += ESPertReadString.substring(9);
    }

    swSerial->write("AT+IBE2?");
    delay(100);
    ESPertReadString = swSerial->readString();

    if (ESPertReadString.length() != 0) {
      _espert->println("ESPert: " + ESPertReadString.substring(9));
      UUID += ESPertReadString.substring(9);
    }

    swSerial->write("AT+IBE3?");
    delay(100);
    ESPertReadString = swSerial->readString();

    if (ESPertReadString.length() != 0) {
      _espert->println("ESPert: " + ESPertReadString.substring(9));
      UUID += ESPertReadString.substring(9);
    }
  }

  return UUID;
}

int ESPert_BLE::getMajor() {
  if (swSerial) {
    swSerial->write("AT+MARJ?");
    delay(100);
    ESPertReadString = swSerial->readString();

    if (ESPertReadString.length() != 0) {
      int l = strtol(ESPertReadString.substring(9).c_str(), NULL, 16);
      return l;
    }
  }

  return -1;
}

int ESPert_BLE::getMinor() {
  if (swSerial) {
    swSerial->write("AT+MINO?");
    delay(100);
    ESPertReadString = swSerial->readString();

    if (ESPertReadString.length() != 0) {
      int l = strtol(ESPertReadString.substring(9).c_str(), NULL, 16);
      return l;
    }
  }

  return -1;
}

int ESPert_BLE::getTXPower() {
  if (swSerial) {
    swSerial->write("AT+MEAS?");
    delay(100);
    ESPertReadString = swSerial->readString();

    if (ESPertReadString.length() != 0) {
      int l = strtol(ESPertReadString.substring(9).c_str(), NULL, 16);
      return l;
    }
  }

  return -1;
}

// ****************************************
// Button class
// ****************************************
ESPert_Button::ESPert_Button() {
  currentButtonStatus = false;
  buttonPressTime = millis();
  buttonPin = ESPERT_PIN_BUTTON;
  isLongPressEnabled = true;
}

void ESPert_Button::init(int pin) {
  if (pin == -1) {
    buttonPin = ESPERT_PIN_BUTTON;
  } else {
    buttonPin = pin;
  }

  pinMode(buttonPin, INPUT_PULLUP);

  currentButtonStatus = false;
  buttonPressTime = millis();
  isLongPressEnabled = true;
}

long ESPert_Button::getPressTime() {
  if (isOn()) {
    return millis() - buttonPressTime;
  }

  return 0l;
}

void ESPert_Button::resetPressTime() {
  buttonPressTime = millis();
}

bool ESPert_Button::isLongPress() {
  return (isLongPressEnabled && getPressTime() >= 2000) ? true : false;
}

void ESPert_Button::enableLongPress() {
  buttonPressTime = millis();
  isLongPressEnabled = true;
}

void ESPert_Button::disableLongPress() {
  buttonPressTime = millis();
  isLongPressEnabled = false;
}

bool ESPert_Button::isOn() {
  int status = digitalRead(buttonPin);
  delay(100);

  bool buttonPressed = (status == LOW) ? true : false;

  if (buttonPressed != currentButtonStatus) {
    if (buttonPressed) {
      buttonPressTime = millis();
    }

    currentButtonStatus = buttonPressed;
  }

  return buttonPressed;
}

bool ESPert_Button::isOff() {
  int status = digitalRead(buttonPin);
  delay(100);
  return (status == HIGH) ? true : false;
}

bool ESPert_Button::get() {
  int status = digitalRead(buttonPin);
  delay(100);
  return (status == HIGH) ? true : false;
}

// ****************************************
// DHT class
// ****************************************
ESPert_DHT::ESPert_DHT() {
  DHT* dht = NULL;
  dhtPin = ESPERT_PIN_DHT;
  dhtType = ESPERT_DHT_TYPE;
}

void ESPert_DHT::init(int gpio, int type, int tx) {
  dhtPin = (gpio == -1) ? ESPERT_PIN_DHT : gpio;
  dhtType = (type == -1) ? ESPERT_DHT_TYPE : type;

  if (tx == -1) {
    tx = (dhtType == DHT22) ? 30 : 15;
  }

  if (!dht) {
    dht = new DHT(dhtPin, dhtType, tx);
    dht->begin();
  }
}

bool ESPert_DHT::isReady() {
  return (dht ? true : false);
}

float ESPert_DHT::getHumidity() {
  float humidity = NAN;
  int retry = 8;

  while (dht && retry) {
    humidity = dht->readHumidity();

    if (isnan(humidity)) {
      if (--retry == 0) {
        _espert->println("ESPert: Failed to read humidity from DHT sensor!");
      }
    } else {
      break;
    }
  }

  return humidity;
}

float ESPert_DHT::getTemperature(bool isFarenheit) {
  float temperature = NAN;
  int retry = 8;

  while (dht && retry) {
    temperature = dht->readTemperature(isFarenheit);

    if (isnan(temperature)) {
      if (--retry == 0) {
        _espert->println("ESPert: Failed to read temperature from DHT sensor!");
      }
    } else {
      break;
    }
  }

  return temperature;
}

// ****************************************
// EEPROM class
// ****************************************
String ESPert_EEPROM::read(int index, int length) {
  String text = "";
  char ch = 1;

  for (int i = index; (i < (index + length)) && ch; ++i) {
    if (ch = EEPROM.read(i)) {
      text.concat(ch);
    }
  }

  return text;
}

int ESPert_EEPROM::write(int index, String text) {
  for (int i = index; i < text.length() + index; ++i) {
    EEPROM.write(i, text[i - index]);
  }

  EEPROM.write(index + text.length(), 0);
  EEPROM.commit();

  return text.length() + 1;
}

// ****************************************
// JSON class
// ****************************************
ESPert_JSON::ESPert_JSON() {
  json = NULL;
  root = NULL;
}

bool ESPert_JSON::init(String payload) {
  release();

  bool success = false;
  unsigned int length = payload.length();

  if (json = (char*)malloc(length)) {
    memset(json, '\0', length);
    memcpy(json, payload.c_str(), length);
    root = &jsonBuffer.parseObject(json);

    if (!(success = root->success())) {
      release();
    }
  }

  return success;
}

bool ESPert_JSON::containsKey(String key) {
  return (json && root->containsKey(key.c_str()));
}

String ESPert_JSON::get(String key) {
  return (json ? (const char*)(*root)[key.c_str()] : "");
}

void ESPert_JSON::release() {
  free(json);
  json = NULL;
  root = NULL;
}

// ****************************************
// LED class
// ****************************************
ESPert_LED::ESPert_LED() {
  ledPin = ESPERT_PIN_LED;
}

void ESPert_LED::init(int pin) {
  if (pin == -1) {
    ledPin = ESPERT_PIN_LED;
  } else {
    ledPin = pin;
  }

  pinMode(ledPin, OUTPUT);
  off();
}

void ESPert_LED::on() {
  set(true);
}

void ESPert_LED::off() {
  set(false);
}

void ESPert_LED::set(bool state) {
  if (ESPertBoardType == ESPERT_BOARD_ESP201) {
    digitalWrite(ledPin, (state ? HIGH : LOW));
  } else {
    digitalWrite(ledPin, (state ? LOW : HIGH));
  }
}

int ESPert_LED::get() {
  if (ESPertBoardType == ESPERT_BOARD_ESP201) {
    return (digitalRead(ledPin) == 1 ? 1 : 0);
  } else {
    return (digitalRead(ledPin) == 1 ? 0 : 1);
  }
}

bool ESPert_LED::isOn() {
  return (get() == 1) ? true : false;
}

bool ESPert_LED::isOff() {
  return (get() == 0) ? true : false;
}

// ****************************************
// OLED class
// ****************************************
ESPert_OLED::ESPert_OLED() {
  display = NULL;
}

void ESPert_OLED::init() {
  if (!display) {
    display = new SSD1306(0x3c, ESPERT_PIN_SDA, ESPERT_PIN_SCL);

#if (SSD1306_LCDHEIGHT != 64)
    //#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

    if (display) {
      // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
      //display->begin(SSD1306_SWITCHCAPVCC, 0x3C, true, ESPERT_PIN_SDA, ESPERT_PIN_SCL);  // initialize with the I2C addr 0x3D (for the 128x64)
      display->init();
      display->flipScreenVertically();

      display->clear();
      drawBitmap(0, 0, logo, 128, 64, ESPERT_WHITE, true);
      delay(1000);
      // init done

      // Show image buffer on the display hardware.
      // Since the buffer is intialized with an Adafruit splashscreen
      // internally, this will display the splashscreen.
      display->display();

      wdt_disable();
      wdt_enable(WDTO_8S);

      setTextSize(1);
      setTextColor(ESPERT_WHITE);
      setCursor(0, 0);
    }
  }
}

bool ESPert_OLED::isReady() {
  return (display ? true : false);
}

#if ARDUINO >= 100
size_t ESPert_OLED::write(uint8_t c)
#else
void ESPert_OLED::write(uint8_t c)
#endif
{
  char t[2];
  t[0] = c;
  t[1] = 0;

  if (c == 13) {
    update();
    cursorX = 0;
    return 1;
  }
  else if (c == 10) {
    cursorY += charHeight;
    return 1;
  }
  display->drawString(cursorX, cursorY, String(t));
  cursorX += charWidth;
  if ((cursorX + charWidth) > maxX) {
    cursorX = 0;
    cursorY += charHeight;
  }

#if ARDUINO >= 100
  return 1;
#endif
}


void ESPert_OLED::clear(bool clearImmediately) {
  if (display) {
    display->clear();
    setCursor(0, 0);

    if (clearImmediately) {
      display->display();
    }
  }
}

void ESPert_OLED::setTextSize(uint8_t s) {
  if (display) {
    //display->setTextSize(s);
    charWidth = 6;
    charHeight = 8;
  }
}

void ESPert_OLED::setTextColor(uint16_t c) {
  if (display) {
    display->setColor(c);
  }
}

void ESPert_OLED::setCursor(int16_t x, int16_t y) {
  cursorX = x;
  cursorY = y;
}

int16_t ESPert_OLED::getCursorX() {
  return cursorX;
}

int16_t ESPert_OLED::getCursorY() {
  return cursorY;
}

void ESPert_OLED::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, uint16_t color, bool drawImmediately) {
  if (display) {
    display->drawBitmap(x, y, w, h, (const char*)bitmap);

    if (drawImmediately) {
      display->display();
    }
  }
}

void ESPert_OLED::update() {
  if (display) {
    display->display();
  }
}



// ****************************************
// MQTT2 class
// ****************************************
ESPert_MQTT2::ESPert_MQTT2()
{
  mqttClient = NULL;
  callback = NULL;
  mqttUser = "";
  mqttPassword = "";
}

void ESPert_MQTT2::init(IPAddress server, int port, String user, String password, MQTT_CALLBACK_SIGNATURE) {
  mqttUser = user;
  mqttPassword = password;
  setCallback(callback);

  if (mqttClient) {
    if (mqttClient->connected())
    {
      mqttClient->disconnect();
    }

    delete mqttClient;
    mqttClient = NULL;
  }

  mqttClient = new PubSubClient(_client);
  mqttClient->setServer(server, 1883);
}

void ESPert_MQTT2::init(IPAddress server, int port, MQTT_CALLBACK_SIGNATURE) {
  init(server, port, "", "", callback);
}

void ESPert_MQTT2::init(const char* server, int port, String user, String password, MQTT_CALLBACK_SIGNATURE) {
  mqttUser = user;
  mqttPassword = password;
  setCallback(callback);

  if (mqttClient) {
    if (mqttClient->connected())
    {
      mqttClient->disconnect();
    }

    delete mqttClient;
    mqttClient = NULL;
  }

  mqttClient = new PubSubClient(_client);
  _espert->println("ESPert: Set Server to " + String(server));
  mqttClient->setServer(server, 1883);
}

void ESPert_MQTT2::init(const char* server, int port, MQTT_CALLBACK_SIGNATURE) {
  init(server, port, "", "", callback);
}

void ESPert_MQTT2::setCallback(MQTT_CALLBACK_SIGNATURE) {
  this->callback = callback;
}

PubSubClient* ESPert_MQTT2::getPubSubClient() {
  return mqttClient;
}

String ESPert_MQTT2::getClientName() {
  String clientName = "";
  uint8_t mac[6] = {0};

  clientName += "ESPert-";
  WiFi.macAddress(mac);
  clientName += _espert->macToString(mac);
  clientName += "-";
  clientName += String(micros() & 0xff, 16);
  return clientName;
}

bool ESPert_MQTT2::connect() {
  bool reconnected = false;

  if (mqttClient && _espert->wifi.getMode() == ESPERT_WIFI_MODE_CONNECT) {
    if (!mqttClient->connected()) {
      String cn = getClientName();

      if (mqttUser.length() > 0) {
        reconnected = mqttClient->connect((const char*)cn.c_str(), mqttUser.c_str(), mqttPassword.c_str());
      } else {
        reconnected = mqttClient->connect((const char*)cn.c_str());
      }

      if (reconnected) {
        if (this->callback) {
          mqttClient->setCallback(this->callback);
        }
      }
    }

    mqttClient->loop();
  }

  return reconnected;
}

void ESPert_MQTT2::publish(String topic, String value) {
  if (mqttClient && _espert->wifi.getMode() == ESPERT_WIFI_MODE_CONNECT) {
    mqttClient->publish(topic.c_str(), (char*)value.c_str());
  }
}

void ESPert_MQTT2::subscribe(String topic) {
  if (mqttClient && _espert->wifi.getMode() == ESPERT_WIFI_MODE_CONNECT) {
    mqttClient->subscribe(topic.c_str());
  }
}

// ****************************************
// WiFi class
// ****************************************
ESPert_WiFi::ESPert_WiFi() {
  wifiMode = ESPERT_WIFI_MODE_DISCONNECT;
}

int ESPert_WiFi::init() {
  WiFi.mode(WIFI_STA);

  boolean bAutoConfig = true;
  String str = _espert->eeprom.read(237, 32);

  if (str == "ESPert:Disconnect") {
    _espert->println("ESPert: Disconnected on startup!");
    WiFi.disconnect();
    delay(100);
    _espert->eeprom.write(237, "ESPert:");
    delay(1000);
  } else if (str == "ESPert:NoAutoConnect") {
    WiFi.disconnect();
    delay(100);
    bAutoConfig = false;
  } else if (str == "ESPert:AutoConnect") {
    bAutoConfig = true;
  } else if (str == "ESPert:SmartConfig") {
    if (smartConfig()) {
      wifiMode = ESPERT_WIFI_MODE_CONNECT;
      _espert->eeprom.write(237, "ESPert:");
      _espert->println("ESPert: WiFi smart config connected, local IP " + _espert->wifi.getLocalIP());
      _espert->button.resetPressTime();
      return wifiMode;
    } else {
      _espert->eeprom.write(237, "ESPert:ConfigAP");
      _espert->println("ESPert: WiFi AP on restart!");

      delay(100);
      ESP.reset();
    }
  } else if (str == "ESPert:ConfigAP") {
    wifiMode = ESPERT_WIFI_MODE_SETTINGAP;

    _espert->led.on();
    delay(50);
    _espert->led.off();
    delay(50);
    _espert->led.on();
    delay(50);
    _espert->led.off();
    delay(50);
    _espert->led.on();
    delay(50);
    _espert->led.off();

    initSetupAP();
    _espert->led.off();

    _espert->println("ESPert: AP & Server started, softAP IP " + _espert->wifi.getAPIP());

    _espert->oled.print("IP..: ");
    _espert->oled.println(getAPIP());
    _espert->oled.print("WiFi: AP Mode");

    int progress = 0;
    int16_t x = _espert->oled.getCursorX();
    int16_t y = _espert->oled.getCursorY();

    while (1) {
      drawProgress(x, y, &progress);

      if (_espert->button.isLongPress()) {
        _espert->button.resetPressTime();
        setAutoConnect(false);
      }

      if (ESPertServer) {
        ESPertServer->handleClient();
        _espert->led.on();
        delay(50);
        _espert->led.off();
        delay(50);
        _espert->led.on();
        delay(50);
        _espert->led.off();
        delay(50);
        _espert->led.on();
        delay(50);
        _espert->led.off();
      }

      delay(1000);
      ESP.wdtFeed();
    }

    _espert->button.resetPressTime();
  }

  if (bAutoConfig) {
    if (test()) {
      wifiMode = ESPERT_WIFI_MODE_CONNECT;
      _espert->println("ESPert: WiFi auto connected, local IP " + _espert->wifi.getLocalIP());
      _espert->button.resetPressTime();
    } else {
      _espert->println("ESPert: Test failed!");
      _espert->eeprom.write(237, "ESPert:SmartConfig");
      _espert->println("ESPert: WiFi SmartConfig on restart!");

      delay(100);
      ESP.reset();
    }
  } else {
    wifiMode = ESPERT_WIFI_MODE_DISCONNECT;
    _espert->println("ESPert: WiFi not connected, local IP " + _espert->wifi.getLocalIP());
    _espert->button.resetPressTime();
  }

  return wifiMode;
}

int ESPert_WiFi::getMode() {
  return wifiMode;
}

bool ESPert_WiFi::smartConfig() {
  _espert->led.on();
  wifiMode = ESPERT_WIFI_MODE_SMARTCONFIG;

  int16_t x = _espert->oled.getCursorX();
  int16_t y = _espert->oled.getCursorY();

  _espert->oled.print("WiFi: Smart Config");

  WiFi.beginSmartConfig();

  int progress = 0;
  x = _espert->oled.getCursorX();
  y = _espert->oled.getCursorY();

  while (1) {
    if (_espert->button.isOn()) {
      if (_espert->button.isLongPress()) {

        _espert->button.resetPressTime();
        drawProgress(x, y, NULL);
        _espert->oled.setCursor(x, y);
        _espert->oled.println();
        WiFi.stopSmartConfig();
        _espert->led.off();
        return false;
      }
    }

    _espert->print("*");
    drawProgress(x, y, &progress);

    if (WiFi.smartConfigDone()) {
      _espert->println();
      _espert->println("ESPert: Smart config success!");

      drawProgress(x, y, NULL);
      _espert->oled.println();
      _espert->oled.print("WiFi: Connecting");

      x = _espert->oled.getCursorX();
      y = _espert->oled.getCursorY();

      progress = 0;
      int timeOut = 10;
      ESP.wdtFeed();

      while (WiFi.status() != WL_CONNECTED) {
        ESP.wdtFeed();

        if (--timeOut <= 0) {
          timeOut = 0;
          break;
        }

        _espert->print("#");
        drawProgress(x, y, &progress);

        delay(500);
        _espert->led.off();
        delay(50);
        _espert->led.on();
      }

      _espert->println();
      drawProgress(x, y, NULL);
      _espert->oled.setCursor(x, y);
      _espert->oled.println();

      if (timeOut == 0) {
        _espert->println("ESPert: WiFi smart connect failed!");
        _espert->oled.println("WiFi: Failed!");
        _espert->led.off();
        ESP.wdtFeed();
        return false;
      } else {
        _espert->println("ESPert: WiFi smart connect success!");
        _espert->oled.println("WiFi: Success!");
      }

      ESP.wdtFeed();
      break;
    }

    delay(500);
    _espert->led.off();
    delay(50);
    _espert->led.on();
    ESP.wdtFeed();
  }

  _espert->led.off();
  return true;
}

void ESPert_WiFi::initSetupAP(void) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  ESPertNumberOfNetworks = WiFi.scanNetworks();

  if (ESPertNumberOfNetworks == 0) {
    _espert->println("ESPert: No networks found!");
  } else {
    int maxNetworks = (sizeof(ESPertNetworks) / sizeof(*ESPertNetworks));

    if (ESPertNumberOfNetworks > maxNetworks) {
      ESPertNumberOfNetworks = maxNetworks;
    }

    _espert->println("ESPert: " + String(ESPertNumberOfNetworks) + " networks found.");

    for (int i = 0; i < ESPertNumberOfNetworks; ++i) {
      ESPertNetworks[i] = String(WiFi.SSID(i));

      _espert->print(i + 1);
      _espert->print(": ");
      _espert->print(WiFi.SSID(i));
      _espert->print(" (");
      _espert->print(WiFi.RSSI(i));
      _espert->print(")");
      _espert->println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");

      ESP.wdtFeed();
    }
  }

  _espert->println();

  WiFi.disconnect();
  delay(100);
  _espert->println("ESPert: Access Point " + _espert->info.getId());
  WiFi.softAP(_espert->info.getId().c_str(), "");
  WiFi.mode(WIFI_AP_STA);

  _espert->println("ESPert: WiFi connected, softAP IP " + _espert->wifi.getAPIP());

  if (ESPertMDNS.begin("ESPert.setup", WiFi.softAPIP())) {
    _espert->println("ESPert: MDNS responder started!");
  } else {
    _espert->println("ESPert: Error setting up MDNS responder!");
  }

  initSetupServer();
}

bool ESPert_WiFi::test(int timeOut) {
  int c = 0;
  _espert->println("ESPert: Waiting for WiFi to connect!");
  _espert->oled.print("WiFi: Connecting");

  int progress = 0;
  int16_t x = _espert->oled.getCursorX();
  int16_t y = _espert->oled.getCursorY();

  while (timeOut == -1 || (timeOut != -1 && c < timeOut)) {
    if (WiFi.status() == WL_CONNECTED) {
      _espert->println();
      _espert->println("ESPert: Connected!");
      _espert->oled.println();
      _espert->oled.println("WiFi: Success!");
      return true;
    }

    drawProgress(x, y, &progress);
    delay(1000);
    int n = WiFi.status();

    if (n == 0) {
      _espert->println("ESPert: No ESP auto connect info!");
      _espert->led.off();
      _espert->oled.println();
      _espert->oled.println("WiFi: No Info!");
      return false;
    }

    _espert->print(n);
    _espert->led.on();
    delay(50);
    _espert->led.off();

    if (timeOut != -1) {
      c++;
    }

    if (_espert->button.isLongPress()) {
      _espert->button.resetPressTime();
      _espert->oled.println();
      return false;
    }

    ESP.wdtFeed();
  }

  _espert->oled.println();
  _espert->oled.println("WiFi: Failed!");

  _espert->println();
  _espert->println("ESPert: Connect timed out!");

  return false;
}

void ESPert_WiFi::initSetupServer() {
  IPAddress ip = WiFi.softAPIP();
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  String title = String("Hello from ESPert at ") + ipStr;

  ESPertContentHeader = "";
  ESPertContentHeader += String("<html>\r\n");
  ESPertContentHeader += String("  <head>\r\n");
  ESPertContentHeader += String("    <title>") + title + " </title>\r\n";
  ESPertContentHeader += String("    <meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n");
  ESPertContentHeader += String("    <meta name='viewport' content='width=device-width, initial-scale=1'>\r\n");
  ESPertContentHeader += String("  </head>\r\n");
  ESPertContentHeader += String("  <body>\r\n");
  ESPertContentHeader += String("    <div align=center>\r\n");
  ESPertContentHeader += String("      <form id='settings' name='settings' action='setting' method='POST'>\r\n");
  ESPertContentHeader += String("        <table cellspacing=0 cellpadding=2 style='border:thin solid black'>\r\n");
  ESPertContentHeader += String("          <tr style='background-color:#666666; min-width:298px; max-width:298px'><td style='min-width:298px; max-width:298px' align=center colspan=2><font color=#ffffff>") + title + "</font></td></tr>\r\n";
  ESP.wdtFeed();

  ESPertContentFooter = "";
  ESPertContentFooter += String("        </table>\r\n");
  ESPertContentFooter += String("      </form>\r\n");
  ESPertContentFooter += String("    </div>\r\n");
  ESPertContentFooter += String("  </body>\r\n");
  ESPertContentFooter += String("</html>\r\n");
  ESP.wdtFeed();

  ESPertSSIDHeader = "";
  ESPertSSIDHeader += String("          <tr style='background-color:#aaaaaa'>\r\n");
  ESPertSSIDHeader += String("            <td align=right>SSID:</td>\r\n");
  ESPertSSIDHeader += String("            <td>\r\n");
  ESPertSSIDHeader += String("              <select id=ssid name=ssid>\r\n");
  ESPertSSIDHeader += String("                <option value="">Choose a Network</option>\r\n");
  ESP.wdtFeed();

  ESPertSSIDFooter = "";
  ESPertSSIDFooter += String("              </select>\r\n");
  ESPertSSIDFooter += String("            </td>\r\n");
  ESPertSSIDFooter += String("          </tr>\r\n");
  ESP.wdtFeed();

  ESPertServer = new ESP8266WebServer(80);

  ESPertServer->on("/", []() {
    ESPertContent = ESPertContentHeader + ESPertSSIDHeader;

    for (int i = 0; i < ESPertNumberOfNetworks; ++i) {
      ESPertContent += String("                <option value='") + ESPertNetworks[i] + "'" + ((ESPertNetworks[i] == ESPertSSID) ? " selected>" : ">") + ESPertNetworks[i] + "</option>\r\n";
      ESP.wdtFeed();
    }

    ESPertContent += ESPertSSIDFooter;
    ESPertContent += String("          <tr style='background-color:#cccccc'><td align=right>Password:</td><td><input type=text id=pass name=pass value='") + ESPertPassword + "'></td></tr>\r\n";
    ESPertContent += String("          <tr><td colspan=2 align=center><input type=submit id=submitButton name=submitButton value='Submit'></td></tr>\r\n");
    ESPertContent += ESPertContentFooter;
    ESP.wdtFeed();

    ESPertServer->send(200, "text/html", ESPertContent);
  });

  ESPertServer->on("/setting", []() {
    ESPertSSID = ESPertServer->arg("ssid");
    ESPertPassword = ESPertServer->arg("pass");
    ESPertSSID.replace("+", " ");
    ESPertSSID.replace("%40", "@");

    if (ESPertSSID.length() > 0) {
      ESPertContent = ESPertContentHeader + ESPertSSIDHeader;

      for (int i = 0; i < ESPertNumberOfNetworks; ++i) {
        ESPertContent += String("                <option value='") + ESPertNetworks[i] + "'" + ((ESPertNetworks[i] == ESPertSSID) ? " selected>" : ">") + ESPertNetworks[i] + "</option>\r\n";
        ESP.wdtFeed();
      }

      ESPertContent += ESPertSSIDFooter;
      ESPertContent += String("          <tr style='background-color:#cccccc'><td align=right>Password:</td><td><input type=text id=pass name=pass value='") + ESPertPassword + "'></td></tr>\r\n";
      ESPertContent += String("          <tr><td colspan=2 align=center>Saved to EEPROM...<br>Reset to boot into new Wi-Fi.</td></tr>\r\n");
      ESPertContent += ESPertContentFooter;
      ESP.wdtFeed();

      ESPertServer->send(200, "text/html", ESPertContent);
      delay(1000);

      WiFi.begin(ESPertSSID.c_str(), ESPertPassword.c_str());
      int c = 0;
      _espert->println("ESPert: Waiting for WiFi to connect!");
      _espert->oled.println();
      _espert->oled.print("WiFi: Connecting");

      int16_t x = _espert->oled.getCursorX();
      int16_t y = _espert->oled.getCursorY();
      int progress = 0;

      while (c < 15) {
        if (WiFi.status() == WL_CONNECTED) {
          _espert->println();
          _espert->println("ESPert: WiFi connected OK, local IP " + _espert->wifi.getLocalIP());
          _espert->eeprom.write(237, "ESPert:");
          delay(100);
          break;
        }

        delay(1000);
        int n = WiFi.status();

        if (n == 0) {
          _espert->println("ESPert: No auto connect available!");

          if (ESPertBoardType == ESPERT_BOARD_ESP201) {
            digitalWrite(ESPERT_PIN_LED, LOW);
          } else {
            digitalWrite(ESPERT_PIN_LED, HIGH);
          }

          break;
        }

        _espert->print(n);
        _espert->wifi.drawProgress(x, y, &progress);

        if (ESPertBoardType == ESPERT_BOARD_ESP201) {
          digitalWrite(ESPERT_PIN_LED, HIGH);
        } else {
          digitalWrite(ESPERT_PIN_LED, LOW);
        }

        delay(50);

        if (ESPertBoardType == ESPERT_BOARD_ESP201) {
          digitalWrite(ESPERT_PIN_LED, LOW);
        } else {
          digitalWrite(ESPERT_PIN_LED, HIGH);
        }

        c++;
        ESP.wdtFeed();
      }

      _espert->wifi.drawProgress(x, y, NULL);
      _espert->oled.setCursor(x, y);
      _espert->oled.println();

      if (WiFi.status() == WL_CONNECTED) {
        _espert->oled.println("WiFi: Success!");
      } else {
        _espert->oled.println("WiFi: Failed!");
      }

      ESP.reset();
    } else {
      ESPertContent = "Error";
      _espert->println("ESPert: Sending 404!");
      ESPertServer->send(200, "text/html", ESPertContent);
    }
  });

  ESPertServer->begin();
  _espert->println("ESPert: HTTP server started!");
}

void ESPert_WiFi::disconnect(bool reset) {
  _espert->println("ESPert: WiFi disconnected!");

  if (ESPertBoardType == ESPERT_BOARD_ESP201) {
    _espert->eeprom.write(237, "ESPert:Disconnect");
    _espert->println("ESPert: WiFi disconnect on restart!");
  } else {
    WiFi.disconnect();
    delay(100);
  }

  if (reset) {
    ESP.reset();
  }
}

void ESPert_WiFi::setAutoConnect(bool bAuto) {
  _espert->print("ESPert: WiFi set auto connect: ");
  _espert->println(bAuto);

  if (bAuto) {
    _espert->eeprom.write(237, "ESPert:AutoConnect");
    _espert->println("ESPert: WiFi auto connect on restart!");
  } else {
    _espert->eeprom.write(237, "ESPert:NoAutoConnect");
    _espert->println("ESPert: WiFi no auto connect on restart!");
  }

  delay(100);
  ESP.reset();
}

String ESPert_WiFi::getLocalIP() {
  IPAddress ip =  WiFi.localIP();
  char textID[32] = {'\0'};
  sprintf(textID, "%i.%i.%i.%i", ip[0], ip[1], ip[2], ip[3]);
  return String(textID);
}

String ESPert_WiFi::getAPIP() {
  IPAddress ip =  WiFi.softAPIP();
  char textID[32] = {'\0'};
  sprintf(textID, "%i.%i.%i.%i", ip[0], ip[1], ip[2], ip[3]);
  return String(textID);
}

void ESPert_WiFi::drawProgress(int16_t x, int16_t y, int* progress) {
  if (_espert->oled.isReady()) {
    _espert->oled.setCursor(x, y);
    _espert->oled.setTextColor(ESPERT_BLACK);
    _espert->oled.print("...");
    _espert->oled.setTextColor(ESPERT_WHITE);
    _espert->oled.setCursor(x, y);

    if (progress) {
      for (int i = 1; i <= *progress; i++) {
        _espert->oled.print(".");
      }

      if (++(*progress) > 3) {
        *progress = 0;
      }
    }

    _espert->oled.update();
    ESP.wdtFeed();
  }
}

String ESPert_WiFi::getHTTP(const char* _host, const char* _path) {
  _espert->print("ESPert: Connecting to ");
  _espert->println(_host);

  WiFiClient client;
  _espert->print("ESPert: Requesting URL: ");
  _espert->print(_host);
  _espert->println(_path);

  JS_HttpClient http(client);
  int err = http.get(_host, _path);

  return httpResponse(&http, err);
}

String ESPert_WiFi::postHTTP(const char* _host, const char* _path) {
  _espert->print("ESPert: Connecting to ");
  _espert->println(_host);

  WiFiClient client;
  _espert->print("ESPert: Requesting URL: ");
  _espert->print(_host);
  _espert->println(_path);

  JS_HttpClient http(client);
  int err = http.post(_host, _path);

  return httpResponse(&http, err);
}

String ESPert_WiFi::httpResponse(JS_HttpClient* http, int err) {
  String response = "";

  if (err == 0) {
    _espert->print("ESPert: Got status code ");
    _espert->println(err);

    // Usually you'd check that the response code is 200 or a
    // similar "success" code (200-299) before carrying on,
    // but we'll print out whatever response we get
    int response_code = http->responseStatusCode();

    _espert->print("ESPert: Got response code ");
    _espert->println(response_code);

    err = http->skipResponseHeaders();
    if (err >= 0) {
      int bodyLen = http->contentLength();
      _espert->print("ESPert: Content length is ");
      _espert->println(bodyLen);
      _espert->print("");
      _espert->println("ESPert: Body returned follows");

      // Now we've got to the body, so we can print it out
      unsigned long timeoutStart = millis();
      char c = 0;
      int iChunkState = eReadingContentLength;
      int iChunkLength = 0;

      // Whilst we haven't timed out & haven't reached the end of the body
      while ((http->connected() || http->available()) && ((millis() - timeoutStart) < kNetworkTimeout)) {
        if (http->available()) {
          c = http->read();

          if (http->isChunk) {
            switch (iChunkState) {
              case eReadingContentLength:
                if (isdigit(c)) {
                  iChunkLength = iChunkLength * 16 + (c - '0');
                } else if (c >= 'a' && c <= 'f') {
                  iChunkLength = iChunkLength * 16 + (c - 'a' + 10);
                } else if (c >= 'A' && c <= 'F') {
                  iChunkLength = iChunkLength * 16 + (c - 'A' + 10);
                } else if (c == '\r') {
                } else {
                  _espert->print("ESPert: Chunk length ");
                  _espert->println(iChunkLength);
                  iChunkState = eReadingBody;
                }
                break;

              case eReadingBody:
                if (iChunkLength > 0) {
                  response = response + c;
                  iChunkLength--;
                  if (iChunkLength == 0)
                    iChunkState = eReadingContentLength;
                }
                break;
            }
          } else {
            response = response + c;
          }

          // Print out this character
          _espert->print(c);
          bodyLen--;
          // We read something, reset the timeout counter
          timeoutStart = millis();
        } else {
          // We haven't got any data, so let's pause to allow some to arrive
          delay(kNetworkDelay);
        }
      }
    } else {
      _espert->print("ESPert: Failed to skip response headers - ");
      _espert->println(err);
    }
  } else {
    _espert->print("ESPert: Connect failed - ");
    _espert->println(err);
  }

  _espert->print("\r\nESPert: Response:\r\n");
  _espert->print(response);
  _espert->println("---\r\n");

  return response;
}

// ****************************************
// GroveButton class
// ****************************************
bool ESPert_GroveButton::isOn() {
  return !ESPert_Button::isOn();
}

bool ESPert_GroveButton::isOff() {
  return !ESPert_Button::isOff();
}

bool ESPert_GroveButton::get() {
  return !ESPert_Button::get();
}

// ****************************************
// GroveLED class
// ****************************************
void ESPert_GroveLED::on() {
  ESPert_LED::set(false);
}

void ESPert_GroveLED::off() {
  ESPert_LED::set(true);
}

void ESPert_GroveLED::set(bool state) {
  ESPert_LED::set(!state);
}

int ESPert_GroveLED::get() {
  return !ESPert_LED::get();
}

// ****************************************
// GroveRelay class
// ****************************************
void ESPert_GroveRelay::on() {
  ESPert_LED::set(false);
}

void ESPert_GroveRelay::off() {
  ESPert_LED::set(true);
}

void ESPert_GroveRelay::set(bool state) {
  ESPert_LED::set(!state);
}

int ESPert_GroveRelay::get() {
  return !ESPert_LED::get();
}

// ****************************************
// Buzzer Class
// ****************************************
void ESPert_Buzzer::init(int pin) {
  this->pin = pin;
}

void ESPert_Buzzer::beep(int freeq, int duration) {
  analogWrite(pin, freeq); // Almost any value can be used except 0 and 255 experiment to get the best tone
  delay(duration);         // wait for a delayms ms
  analogWrite(pin, 0);     // 0 turns it off
}

void ESPert_Buzzer::on(int freeq) {
  analogWrite(pin, freeq); // Almost any value can be used except 0 and 255 experiment to get the best tone
}

void ESPert_Buzzer::off() {
  analogWrite(pin, 0); // 0 turns it off
}

// ****************************************
// NeoPixel Class
// ****************************************
void ESPERT_NeoPixel::init(uint8_t p, uint8_t n) {
  if (!_neopixel) {
    this->_neopixel = new Adafruit_NeoPixel(n , p , NEO_GRB + NEO_KHZ800);
  }

  this->_neopixel->begin();
  this->_neopixel->show();
}

void ESPERT_NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  this->_neopixel->setPixelColor(n, r, g, b);
}

void ESPERT_NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  this->_neopixel->setPixelColor(n, r, g, b, w);
}

void ESPERT_NeoPixel::setPixelColor(uint16_t n, uint32_t c) {
  this->_neopixel->setPixelColor(n, c);
}

void ESPERT_NeoPixel::show() {
  this->_neopixel->show();
}

void ESPERT_NeoPixel::clear() {
  this->_neopixel->clear();
}

void ESPERT_NeoPixel::off() {
  this->_neopixel->clear();
  this->_neopixel->show();
}

void ESPERT_NeoPixel::setColor(uint8_t r, uint8_t g, uint8_t b) {
  for (uint8_t i = 0; i < this->_neopixel->numPixels() ; i++) {
    this->_neopixel->setPixelColor(i, r, g, b);
  }

  this->_neopixel->show();
}

void ESPERT_NeoPixel::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  for (uint8_t i = 0; i < this->_neopixel->numPixels() ; i++) {
    this->_neopixel->setPixelColor(i, r, g, b, w);
  }
}

void ESPERT_NeoPixel::setColor(uint32_t c) {
  for (uint8_t i = 0; i < this->_neopixel->numPixels() ; i++) {
    this->_neopixel->setPixelColor(i, c);
  }

  this->_neopixel->show();
}

void ESPERT_NeoPixel::rainbow() {
  for (uint8_t i = 0; i < this->_neopixel->numPixels(); i++) {
    this->_neopixel->setPixelColor(i, Wheel(((i * 256 / this->_neopixel->numPixels())) & 255));
  }

  this->_neopixel->show();
}

uint32_t ESPERT_NeoPixel::Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;

  if (WheelPos < 85) {
    return this->_neopixel->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }

  if (WheelPos < 170) {
    WheelPos -= 85;
    return this->_neopixel->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }

  WheelPos -= 170;
  return this->_neopixel->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
