#ifndef __ESPERT_h__
#define __ESPERT_h__

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <functional>
#include <Print.h>
#include <ESP_Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <PubSubClient.H>

static const float ESPERT_LIBRARY_VERSION = 0.8f;

#define ESPERT_BOARD_GENERIC            0
#define ESPERT_BOARD_ESPRESSO_LITE      1
#define ESPERT_BOARD_ESP201             2

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

#define ESPERT_BLACK 0
#define ESPERT_WHITE 1
#define ESPERT_INVERSE 2

#define ESPERT_WIFI_MODE_DISCONNECT  0
#define ESPERT_WIFI_MODE_CONNECT     1
#define ESPERT_WIFI_MODE_SMARTCONFIG 2
#define ESPERT_WIFI_MODE_SETTINGAP   3

#define MAX_NETWORKS 32

class ESPert_WiFi 
{
public:
    int  wifiMode;

    int  init();
    int  getMode() { return wifiMode; };
    void setAutoConnect( bool bAuto );
    bool smartConfig();
    void initSetupAP(void);
    void initSetupServer();
    bool test(void);
    void disconnect( bool reset=true);
    String getLocalIP();
    String getAPIP();
};

class ESPert_LED
{
    private:
        int pin_LED;
        
    public:
        void init( int pin=-1);
        void on();
        void off();
        void set( bool state );
        int get();
};

class ESPert_Button
{
    private:
        bool currentButtonStatus;
        long buttonPressTime;

        void resetPressTime();
        int  pin_button;
        
    public:
        long getPressTime();
        void init( int pin=-1 );
        virtual bool isOn();
        virtual bool isOff();
        virtual bool get();
        bool isLongPress();
};

class ESPert_GroveButton: public ESPert_Button
{
    private:
    public:
        virtual bool isOn();
        virtual bool isOff();
        virtual bool get();
};

class ESPert_GroveRelay: public ESPert_LED
{
    public:
        void on();
        void off();
        void set( bool state );
        int get();
};


class ESPert_OLED
{
  private:
    Adafruit_SSD1306 *display;

  public:
    void init();
    bool isReady();
    void clear();
    void setTextSize(uint8_t s);
    void setTextColor(uint16_t c);
    void setTextColor(uint16_t c, uint16_t bg);
    void setCursor(int16_t x, int16_t y);
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
    void print(const String &s);
    void print(double f, int p = 2);
    void print(int i);
    void println(const String &s);
    void println(double f, int p = 2);
    void println(int i);
};

class ESPert_DHT
{
  private:
    bool hutmp;
    DHT *dht;
    int pin_dht;
    int dht_type;

  public:
    void init( int pin=-1, int type=-1 );
    bool isReady();
    int getHumidity();
    int getTemperature();
};

class ESPert_SoftwareSerial : public Stream
{
    public:
        ESPert_SoftwareSerial();
        void init( int rx=12, int tx=14, int buffer=128 );
        String readString();
        //void write( String str );
        virtual size_t write(uint8_t byte);
        virtual int read();
        virtual int available();
        virtual void flush();
        int peek();

        void begin( int baud=9600 );
        
        SoftwareSerial  *swSerial;
        
        using Print::write;

};

class ESPert_BLE 
{
    ESPert_SoftwareSerial *swSerial;
    
    public:
        bool init( ESPert_SoftwareSerial *swSer );
        String getFirmwareVersion();
        bool isOn();
        bool on();
        bool off();
        
        String getUUID();
        int getMajor();
        int getMinor();
        int getTXPower();

};

class ESPert_MQTT
{
    PubSubClient::callback_t callback;
    
    public:
        void init( IPAddress server, int port );
        void init( String server, int port );
        void setCallback( PubSubClient::callback_t cb=NULL );
        
        String getClientName();
        void publish( String topic, String value );
        void subscribe( String topic );
        void connect();
        PubSubClient * getPubSubClient(); 
};


class ESPert : public Print
{
    private:
    
    public:
        ESPert_LED      LED;
        ESPert_Button   button;
        ESPert_OLED     OLED;
        ESPert_DHT      DHT;
        ESPert_WiFi     wifi;
        ESPert_SoftwareSerial   swSerial;
        ESPert_BLE              ble;
        ESPert_MQTT             mqtt;
        
        String EEPROM_Read( int index, int length );
        int EEPROM_Write( int index, String text );

    public:
        ESPert();
        void init( int type = ESPERT_BOARD_ESPRESSO_LITE );
        void loop();
    
        #if ARDUINO >= 100
            virtual size_t write(uint8_t);
        #else
            virtual void   write(uint8_t);
        #endif     
        
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


#endif //__ESPERT_h__