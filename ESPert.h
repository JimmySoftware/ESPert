#ifndef __ESPERT_h__
#define __ESPERT_h__

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <functional>
#include <Print.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <JS_HttpClient.h>
#include <SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "logo.h"

static const float ESPERT_LIBRARY_VERSION = 0.8f;

#define ESPERT_BOARD_GENERIC        0
#define ESPERT_BOARD_ESP201         1
#define ESPERT_BOARD_ESPRESSO_LITE  2
#define ESPERT_BOARD_ESPRESSO_LITE2 3

static int ESPertBoardType = ESPERT_BOARD_ESPRESSO_LITE; // default

#ifdef ESPERT_DEBUG

#ifndef ESPERT_DEBUG_PRINTER
#define ESPERT_DEBUG_PRINTER Serial
#endif // ESPERT_DEBUG_PRINTER

#ifdef ESPERT_DEBUG_LEVEL_VERBOSE
#define ESPERT_INFO_PRINT(...) {ESPERT_DEBUG_PRINTER.print(__VA_ARGS__);}
#define ESPERT_INFO_PRINTLN(...) {ESPERT_DEBUG_PRINTER.println(__VA_ARGS__);}
#else // !ESPERT_DEBUG_LEVEL_VERBOSE
#define ESPERT_INFO_PRINT(...) {}
#define ESPERT_INFO_PRINTLN(...) {}
#endif

#define ESPERT_DEBUG_PRINT(...) {ESPERT_DEBUG_PRINTER.print(__VA_ARGS__);}
#define ESPERT_DEBUG_PRINTLN(...) {ESPERT_DEBUG_PRINTER.println(__VA_ARGS__);}

#else // !ESPERT_DEBUG

#define ESPERT_DEBUG_PRINT(...) {}
#define ESPERT_DEBUG_PRINTLN(...) {}

#define ESPERT_INFO_PRINT(...) {}
#define ESPERT_INFO_PRINTLN(...) {}

#endif // ESPERT_DEBUG

#define ESPERT_BLACK   0
#define ESPERT_WHITE   1
#define ESPERT_INVERSE 2

#define ESPERT_WIFI_MODE_DISCONNECT  0
#define ESPERT_WIFI_MODE_CONNECT     1
#define ESPERT_WIFI_MODE_SMARTCONFIG 2
#define ESPERT_WIFI_MODE_SETTINGAP   3

#define ColorRed    0xFF0000
#define ColorGreen  0x008000
#define ColorBlue   0x0000FF
#define ColorCyan   0x00FFFF
#define ColorPurple 0x800080
#define ColorGray   0x808080
#define ColorBrown  0xA52A2A
#define ColorSilver 0xC0C0C0
#define ColorViolet 0xEE82EE
#define ColorPink   0xFFC0CB
#define ColorGold   0xFFD700
#define ColorYellow 0xFFFF00
#define ColorWhite  0xFFFFFF

static int ESPERT_PIN_LED    = 16;
static int ESPERT_PIN_BUTTON = 2;
static int ESPERT_PIN_SDA    = 4;
static int ESPERT_PIN_SCL    = 5;
static int ESPERT_PIN_DHT    = 12;

static int ESPERT_DHT_TYPE   = DHT22;

static const long ESPertFlashID[] = {0x1640EF, 0x1340C8, 0x1340EF}; // Little Endian
static const String ESPertFlashDesc[] = {"WINBOND W25Q32: 32M-bit / 4M-byte", "GIGADEVICE GD25Q40 4M-bit / 512K-byte", "WINBOND W25Q40 4M-bit / 512K-byte"};

static ESP8266WebServer* ESPertServer = NULL;
static MDNSResponder ESPertMDNS;

static String ESPertNetworks[32] = {""};
static int ESPertNumberOfNetworks = 0;
static String ESPertSSID = "";
static String ESPertPassword = "";
static String ESPertContentHeader = "";
static String ESPertContentFooter = "";
static String ESPertSSIDHeader = "";
static String ESPertSSIDFooter = "";
static String ESPertContent = "";

static String ESPertReadString; // rx tx buffer

typedef enum {
  eIdle,
  eRequestStarted,
  eRequestSent,
  eReadingStatusCode,
  eStatusCodeRead,
  eReadingContentLength,
  eSkipToEndOfHeader,
  eLineStartingCRFound,
  eReadingBody
} tHttpState;

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30 * 1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

class ESPert_SoftwareSerial : public Stream
{
  public:
    SoftwareSerial* swSerial;
    using Print::write;
    ESPert_SoftwareSerial();
    void init(int rx = 12, int tx = 14, int buffer = 128);
    String readString();
    virtual size_t write(uint8_t byte);
    virtual int read();
    virtual int available();
    virtual void flush();
    int peek();
    void begin(int baud = 9600);
};

class ESPert_BLE
{
  private:
    ESPert_SoftwareSerial* swSerial;

  public:
    ESPert_BLE();
    bool init(ESPert_SoftwareSerial* swSer);
    String getFirmwareVersion();
    bool isOn();
    bool isOff();
    bool on();
    bool off();
    String getUUID();
    int getMajor();
    int getMinor();
    int getTXPower();
};

class ESPert_Button
{
  private:
    bool currentButtonStatus;
    long buttonPressTime;
    int buttonPin;
    bool isLongPressEnabled;

  public:
    ESPert_Button();
    void init(int pin = -1);
    long getPressTime();
    void resetPressTime();
    bool isLongPress();
    void enableLongPress();
    void disableLongPress();
    virtual bool isOn();
    virtual bool isOff();
    virtual bool get();
};

class ESPert_DHT
{
  private:
    DHT* dht;
    int dhtPin;
    int dhtType;

  public:
    ESPert_DHT();
    void init(int gpio = -1, int type = -1, int tx = -1);
    bool isReady();
    float getHumidity();
    float getTemperature(bool isFarenheit = false);
};

class ESPert_EEPROM
{
  public:
    String read(int index, int length);
    int write(int index, String text);
};

class ESPert_Info
{
  public:
    String getId();
    float getLibraryVersion();
    uint32_t getFreeHeap();
    uint32_t getChipId();
    uint32_t getFlashChipId();
    String getFlashChipInfo();
    uint32_t getFlashChipRealSize();
    uint32_t getFlashChipSize();
    uint32_t getFlashChipSpeed();
    uint32_t getFlashChipSizeByChipId();

    inline uint32_t getCycleCount() {
      return ESP.getCycleCount();
    };
};

class ESPert_JSON
{
  private:
    char* json;
    JsonObject* root;
    DynamicJsonBuffer jsonBuffer;

  public:
    ESPert_JSON();
    bool init(String payload);
    bool containsKey(String key);
    String get(String key);
    void release();
};

class ESPert_LED
{
  private:
    int ledPin;

  public:
    ESPert_LED();
    void init(int pin = -1);
    void on();
    void off();
    void set(bool state);
    int get();
    bool isOn();
    bool isOff();
};

class ESPert_OLED : public Print
{
  private:
    SSD1306* display;

  public:
    ESPert_OLED();
    void init();
    bool isReady();
    void clear(bool clearImmediately = true);
    void setTextSize(uint8_t s);
    void setTextColor(uint16_t c);
    void setCursor(int16_t x, int16_t y);
    int16_t getCursorX();
    int16_t getCursorY();
    void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, uint16_t color, bool drawImmediately = true);
    void update();

    int cursorX;
    int cursorY;

    int charWidth;
    int charHeight;

    const int maxX = 128;
    const int maxY = 64;

#if ARDUINO >= 100
    virtual size_t write(uint8_t);
#else
    virtual void write(uint8_t);
#endif
};

class ESPert_MQTT2
{
  private:
    PubSubClient* mqttClient = NULL;
    MQTT_CALLBACK_SIGNATURE;
    String mqttUser;
    String mqttPassword;
    WiFiClient _client;

  public:
    ESPert_MQTT2();
    void init(IPAddress server, int port, String user = "", String password = "", MQTT_CALLBACK_SIGNATURE = NULL);
    void init(IPAddress server, int port, MQTT_CALLBACK_SIGNATURE);
    void init(const char* server, int port, String user = "", String password = "", MQTT_CALLBACK_SIGNATURE = NULL);
    void init(const char* server, int port, MQTT_CALLBACK_SIGNATURE);

    void setCallback(MQTT_CALLBACK_SIGNATURE = NULL);

    String getClientName();
    PubSubClient* getPubSubClient();
    void publish(String topic, String value);
    void subscribe(String topic);
    bool connect();
};

class ESPert_WiFi
{
  private:
    int wifiMode;
    void drawProgress(int16_t x, int16_t y, int* progress = NULL);
    String httpResponse(JS_HttpClient* http, int err);

  public:
    ESPert_WiFi();
    int init();
    int getMode();
    void setAutoConnect(bool bAuto);
    bool smartConfig();
    void initSetupAP(void);
    void initSetupServer();
    bool test(int timeOut = -1);
    void disconnect(bool reset = true);
    String getLocalIP();
    String getAPIP();

    String getHTTP(const char* host, const char* path);
    String postHTTP(const char* host, const char* path);
};

class ESPert_GroveButton: public ESPert_Button
{
  public:
    virtual bool isOn();
    virtual bool isOff();
    virtual bool get();
};

class ESPert_GroveLED: public ESPert_LED
{
  public:
    void on();
    void off();
    void set(bool state);
    int get();
};

class ESPert_GroveRelay: public ESPert_LED
{
  public:
    void on();
    void off();
    void set(bool state);
    int get();
};

class ESPert_Grove
{
  public:
    ESPert_GroveButton button;
    ESPert_GroveLED    led;
    ESPert_GroveRelay  relay;
};

class ESPert_Buzzer
{
  private:
    int pin;

  public:
    void init(int pin = -1);
    void beep(int freeq, int duration);
    void on(int freq);
    void off();
};

class ESPERT_NeoPixel // WS2812
{
  private:
    Adafruit_NeoPixel* _neopixel;

  public:
    void init(uint8_t p = 14, uint8_t n = 8);
    void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
    void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    void setPixelColor(uint16_t n, uint32_t c);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    void setColor(uint32_t c);
    void clear();
    void off();
    void show();
    void rainbow();
    uint32_t Wheel(byte WheelPos);
};

class ESPert : public Print
{
  public:
    ESPert_BLE            ble;
    ESPert_Button         button;
    ESPert_DHT            dht;
    ESPert_EEPROM         eeprom;
    ESPert_Grove          grove;
    ESPert_Info           info;
    ESPert_JSON           json;
    ESPert_LED            led;
    ESPert_OLED           oled;
    ESPert_MQTT2          mqtt;
    ESPert_SoftwareSerial swSerial;
    ESPert_WiFi           wifi;
    ESPert_Buzzer         buzzer;
    ESPERT_NeoPixel  	    neopixel;

    ESPert();
    void init(int type = -1, long baud = 115200);
    void loop();
    String macToString(const uint8_t* mac);
    bool checkFlashSize();

#if ARDUINO >= 100
    virtual size_t write(uint8_t);
#else
    virtual void write(uint8_t);
#endif
};

#endif //__ESPERT_h__
