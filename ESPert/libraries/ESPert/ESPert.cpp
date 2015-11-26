#include "ESPert.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

static int _boardType = ESPERT_BOARD_GENERIC;

static int ESPERT_PIN_LED = 16;
static int ESPERT_PIN_BUTTON = 2;
static int ESPERT_PIN_SDA = 4;
static int ESPERT_PIN_SCL = 5;
static int ESPERT_PIN_DHT = 12;

static int ESPERT_DHT_TYPE = DHT22;

static const long ESPertFlashID[3] = {0x1640EF, 0x1340C8, 0x1340EF}; // Little Endian
static const String ESPertFlashDesc[3] = {"WINBOND W25Q32: 32M-bit / 4M-byte", "GIGADEVICE GD25Q40 4M-bit / 512K-byte", "WINBOND W25Q40 4M-bit / 512K-byte"};

static ESPert *_espert;
static ESP8266WebServer *server = NULL;
static MDNSResponder mdns;

static String networks[MAX_NETWORKS] = {""};
static int numberOfNetworks = 0;

static String ssid = "";
static String pass = "";
static String contentHeader = "";
static String contentFooter = "";
static String ssidHeader = "";
static String ssidFooter = "";
static String content = "";

// rx tx buffer
static String _readString;

static PubSubClient *mqtt_client = NULL;

ESPert::ESPert()
{
    _espert = this;
}

void ESPert::init(int type)
{
    Serial.begin(115200);
    Serial.println();

    _boardType = type;
    
    wdt_disable();
    wdt_enable(WDTO_8S);

    LED.init();
    
    button.init();
}

void ESPert::loop() {
  if( button.isLongPress() ) {
    println( "Button: Long Pressed" );
    int mode = wifi.getMode();
    if( mode == ESPERT_WIFI_MODE_CONNECT ) {
      wifi.setAutoConnect( false );
    }
    else if( mode == ESPERT_WIFI_MODE_DISCONNECT ) {
      wifi.setAutoConnect( true );
    }
  }  
  if( mqtt_client ) {  
    mqtt.connect();
  }

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
        Serial.print( (char)c );
    }
#if ARDUINO >= 100
    return 1;
#endif
}

// ****************************************
// LED class
// ****************************************
void ESPert_LED::init( int pin ) {
    if( pin == -1 )
        pin_LED = ESPERT_PIN_LED;
    else
        pin_LED = pin;
        
    pinMode( pin_LED, OUTPUT );
    off();
}

void ESPert_LED::on() {
    set(true);
}

void ESPert_LED::off() {
    set(false);
}

void ESPert_LED::set(bool state) {
    if (_boardType == ESPERT_BOARD_ESP201) {
        digitalWrite(pin_LED, (state ? HIGH : LOW));
    } else {
        digitalWrite(pin_LED, (state ? LOW : HIGH));
    }
}

int ESPert_LED::get() {
    if (_boardType == ESPERT_BOARD_ESP201) {
        return( digitalRead(pin_LED) == 1 ? 1 : 0 );
    } else {
        return( digitalRead(pin_LED) == 1 ? 0 : 1 );
    }
}

// ****************************************
// Device Info
// ****************************************
String ESPert::getId() {
  char textID[16];
  sprintf(textID, "ESPert-%lu", ESP.getChipId());
  return String(textID);
}

float ESPert::getLibraryVersion() {
  return ESPERT_LIBRARY_VERSION;
}

uint32_t ESPert::getFreeHeap() {
  return ESP.getFreeHeap();
}

uint32_t ESPert::getChipId() {
  return ESP.getChipId();
}

uint32_t ESPert::getFlashChipId() {
  return ESP.getFlashChipId();
}

String ESPert::getFlashChipInfo() {
  int flashChipID = ESP.getFlashChipId();
  ESPERT_INFO_PRINT(flashChipID, HEX);

  for (int i = 0; i < 3; i++) {
    if (ESP.getFlashChipId() == ESPertFlashID[i]) {
      return ESPertFlashDesc[i];
    }
  }

  return "(Unknown Flash Chip)";
}

uint32_t ESPert::getFlashChipRealSize() {
  return ESP.getFlashChipRealSize();
}

uint32_t ESPert::getFlashChipSize() {
  return ESP.getFlashChipSize();
}

uint32_t ESPert::getFlashChipSpeed() {
  return ESP.getFlashChipSpeed();
}

uint32_t ESPert::getFlashChipSizeByChipId() {
  return ESP.getFlashChipSizeByChipId();
}

String macToStr(const uint8_t* mac) {
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
// button class
// ****************************************
void ESPert_Button::init( int pin ) {
    if( pin == -1 )
        pin_button = ESPERT_PIN_BUTTON;
    else
        pin_button = pin;
        
    pinMode( pin_button, INPUT_PULLUP);

    currentButtonStatus = false;
    buttonPressTime = millis();
}

long ESPert_Button::getPressTime() {
    if( isOn() ) {
        return millis() - buttonPressTime;
    }

    return 0l;
}

void ESPert_Button::resetPressTime() {
    buttonPressTime = millis();
}

bool ESPert_Button::isLongPress() {
    return( getPressTime() >= 2000 ) ? true : false;
}

bool ESPert_Button::isOn() {
    bool buttonPressed = false;

    buttonPressed = (digitalRead(pin_button) == LOW) ? true : false;

    if (buttonPressed != currentButtonStatus) {
        if (buttonPressed) {
            buttonPressTime = millis();
            ESPERT_INFO_PRINT("Button: ON");
        } else {
            ESPERT_INFO_PRINT("Button: OFF");
        }
        currentButtonStatus = buttonPressed;
    }

    return buttonPressed;
}

bool ESPert_Button::isOff() {
    return (digitalRead(pin_button) == HIGH) ? true : false;
}

bool ESPert_Button::get() {
    return (digitalRead(pin_button) == HIGH) ? true : false;
}

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
// OLED class
// ****************************************
void ESPert_OLED::init() {
  if (!isReady()) {
    display = new Adafruit_SSD1306(4);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

    if (display) {
      // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
      display->begin(SSD1306_SWITCHCAPVCC, 0x3C, true, ESPERT_PIN_SDA, ESPERT_PIN_SCL);  // initialize with the I2C addr 0x3D (for the 128x64)
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

void ESPert_OLED::clear() {
  if (display) {
    display->clearDisplay();
    display->display();
    setCursor(0, 0);
  }
}

void ESPert_OLED::setTextSize(uint8_t s) {
  if (display) {
    display->setTextSize(s);
  }
}

void ESPert_OLED::setTextColor(uint16_t c) {
  if (display) {
    display->setTextColor(c);
  }
}

void ESPert_OLED::setTextColor(uint16_t c, uint16_t bg) {
  if (display) {
    display->setTextColor(c, bg);
  }
}

void ESPert_OLED::setCursor(int16_t x, int16_t y) {
  if (display) {
    display->setCursor(x, y);
  }
}

void ESPert_OLED::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {
  if (display) {
    display->drawBitmap(x, y,  bitmap, w, h, color);
  }
}

void ESPert_OLED::print(const String &s) {
  if (display) {
    display->print(s);
    display->display();
  }
}

void ESPert_OLED::print(double f, int p) {
  if (display) {
    display->print(f, p);
    display->display();
  }
}

void ESPert_OLED::print(int i) {
    if (display) {
        display->print(i);
        display->display();
    }
}

void ESPert_OLED::println(const String &s) {
  if (display) {
    display->println(s);
    display->display();
  }
}

void ESPert_OLED::println(double f, int p) {
  if (display) {
    display->println(f, p);
    display->display();
  }
}

void ESPert_OLED::println(int i) {
    if (display) {
        display->println(i);
        display->display();
    }
}

// ****************************************
// HUTMP class
// ****************************************
void ESPert_DHT::init( int pin, int type ) {
    if( pin == -1 )
        pin_dht = ESPERT_PIN_DHT;
    else
        pin_dht = pin;
        
    if( type == -1 )
        dht_type = ESPERT_DHT_TYPE;
    else
        dht_type = type;
        
    int tx = 15;
    if( dht_type == DHT22 ) 
        tx = 30;
        
  if (!isReady()) {
    dht = NULL;

    dht = new DHT(pin_dht, dht_type, tx);
    dht->begin();
  }
}

bool ESPert_DHT::isReady() {
    return (dht ? true : false);
}

int ESPert_DHT::getHumidity() {
    return dht->readHumidity();
}

int ESPert_DHT::getTemperature() {
    return dht->readTemperature();
}

// *******************************************
// WiFi
// *******************************************
int  ESPert_WiFi::init()
{
    boolean bAutoConfig = true;
    
    WiFi.mode(WIFI_STA);

    String str = _espert->EEPROM_Read( 237, 32 );
    {
        if( str == "TESPA:Disconnect" ) {
            Serial.println("Disconnected on startup");
            WiFi.disconnect();
            delay( 100 );
            _espert->EEPROM_Write( 237, "TESPA:" );
            delay( 1000 );
        }
        if( str == "TESPA:NoAutoConnect" ) {
            WiFi.disconnect();
            delay( 100 );
            bAutoConfig = false;
        }
        if( str == "TESPA:AutoConnect" ) {
            bAutoConfig = true;
        }
    }
  
    if( bAutoConfig ) {
        if( !test() )
        {
            if( !smartConfig() ) {
                wifiMode = ESPERT_WIFI_MODE_SETTINGAP;

                // LED: ... ... ...                        
                _espert->LED.on();
                delay(50);
                _espert->LED.off();
                delay(50);
                _espert->LED.on();
                delay(50);
                _espert->LED.off();
                delay(50);
                _espert->LED.on();
                delay(50);
                _espert->LED.off();
                                        
                initSetupAP();
                _espert->LED.off();
                Serial.println("AP & Server started");
                Serial.println(WiFi.softAPIP());
                if( _espert->OLED.isReady() ) {
                    _espert->OLED.println( "WiFi: Setting AP" );
                    _espert->OLED.print( "IP: " );
                    _espert->OLED.println( getAPIP() );    
                }
                while( 1 ) {
                    if( _espert->button.isLongPress() ) {
                        setAutoConnect( false );
                    }
                    if ( server ) {
                        server->handleClient();
                        _espert->LED.on();
                        delay(50);
                        _espert->LED.off();
                        delay(50);
                        _espert->LED.on();
                        delay(50);
                        _espert->LED.off();
                        delay(50);
                        _espert->LED.on();
                        delay(50);
                        _espert->LED.off();
                    }                    
                    delay(1000);
                }
                return ESPERT_WIFI_MODE_SETTINGAP;
            }
            else {
                wifiMode = ESPERT_WIFI_MODE_CONNECT;
                Serial.println("WiFi connected");
                Serial.println(WiFi.localIP());

                Serial.println("WiFi smart config connected");
                Serial.println(WiFi.localIP());
                WiFi.printDiag(Serial);

                _espert->OLED.clear();
                _espert->OLED.setCursor( 0, 0 );
                _espert->OLED.println( _espert->getId() );
                _espert->OLED.println( "" );           

                return ESPERT_WIFI_MODE_CONNECT;
            }
        }
        else {
            wifiMode = ESPERT_WIFI_MODE_CONNECT;
            Serial.println("WiFi auto connected");
            Serial.println(WiFi.localIP());
            WiFi.printDiag(Serial);
            return ESPERT_WIFI_MODE_CONNECT;
        }
    }
    else {
        wifiMode = ESPERT_WIFI_MODE_DISCONNECT;
        Serial.println("WiFi not connected");
        Serial.println(WiFi.localIP());
        return ESPERT_WIFI_MODE_DISCONNECT;
    }
}

bool ESPert_WiFi::smartConfig()
{
  _espert->LED.on();
  wifiMode = ESPERT_WIFI_MODE_SMARTCONFIG;
  
  if( _espert->OLED.isReady() ) {
    _espert->OLED.println("WiFi: Smart Config" );
  }
  
  WiFi.beginSmartConfig();
  while(1){
    if( _espert->button.isOn() ) {
      long pt = _espert->button.getPressTime();
      if( pt > 2000 ) {
        WiFi.stopSmartConfig();
        _espert->LED.off();
        return false;
      }
    }
    Serial.print("*");
    if(WiFi.smartConfigDone()){
      Serial.println( "" );
      Serial.println("SmartConfig Success");
      while(WiFi.waitForConnectResult() != WL_CONNECTED){
        Serial.print( "#" );        
        delay( 500 );      
        _espert->LED.off();
        delay( 50 );
        _espert->LED.on();
      }
      Serial.println( "" );
      Serial.println( "WiFi Smart connect success." );              
      break;
    }
    delay( 500 );
    _espert->LED.off();
    delay( 50 );
    _espert->LED.on();
  }
  _espert->LED.off();
  return true;
}

void ESPert_WiFi::initSetupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  numberOfNetworks = WiFi.scanNetworks();
  Serial.println("scan done");
  if (numberOfNetworks == 0)
    Serial.println("no networks found");
  else
  {
    if (numberOfNetworks > MAX_NETWORKS)
    {
      numberOfNetworks = MAX_NETWORKS;
    }
    Serial.print(numberOfNetworks);
    Serial.println(" networks found");
    for (int i = 0; i < numberOfNetworks; ++i)
     {
      networks[i] = String(WiFi.SSID(i));

      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
     }
  }
  Serial.println("");

  delay(100);
  WiFi.softAP(_espert->getId().c_str());
  Serial.println("softap");

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());

  if (!mdns.begin("tespa.setup", WiFi.softAPIP())) {
    Serial.println("Error setting up MDNS responder!");
  }
  else {
    Serial.println("mDNS responder started");
  }
  initSetupServer();
  Serial.println("over");
}


bool ESPert_WiFi::test(void) {
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 15 ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println( "" );
      Serial.println("Connected.");
      return true;
    }
    delay(1000);
    int n = WiFi.status();
    if( n == 0 )
    {
        Serial.println("No ESP auto connect info");
        _espert->LED.off();
        return false;
    } 
    Serial.print(n);
    _espert->LED.on();
    delay( 50 );
    _espert->LED.off();
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out.");
  return false;
}

void ESPert_WiFi::initSetupServer()
{
        IPAddress ip = WiFi.softAPIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  String title = String("Hello from TESPA at ") + ipStr;

  contentHeader = "";
  contentHeader += String("<html>\r\n");
  contentHeader += String("  <head>\r\n");
  contentHeader += String("    <title>") + title + " </title>\r\n";
  contentHeader += String("    <meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n");
  contentHeader += String("    <meta name='viewport' content='width=device-width, initial-scale=1'>\r\n");
  contentHeader += String("  </head>\r\n");
  contentHeader += String("  <body>\r\n");
  contentHeader += String("    <div align=center>\r\n");
  contentHeader += String("      <form id='settings' name='settings' action='setting' method='POST'>\r\n");
  contentHeader += String("        <table cellspacing=0 cellpadding=2 style='border:thin solid black'>\r\n");
  contentHeader += String("          <tr style='background-color:#666666; min-width:298px; max-width:298px'><td style='min-width:298px; max-width:298px' align=center colspan=2><font color=#ffffff>") + title + "</font></td></tr>\r\n";

  contentFooter = "";
  contentFooter += String("        </table>\r\n");
  contentFooter += String("      </form>\r\n");
  contentFooter += String("    </div>\r\n");
  contentFooter += String("  </body>\r\n");
  contentFooter += String("</html>\r\n");

  ssidHeader = "";
  ssidHeader += String("          <tr style='background-color:#aaaaaa'>\r\n");
  ssidHeader += String("            <td align=right>SSID:</td>\r\n");
  ssidHeader += String("            <td>\r\n");
  ssidHeader += String("              <select id=ssid name=ssid>\r\n");
  ssidHeader += String("                <option value="">Choose a Network</option>\r\n");

  ssidFooter = "";
  ssidFooter += String("              </select>\r\n");
  ssidFooter += String("            </td>\r\n");
  ssidFooter += String("          </tr>\r\n");

  server = new ESP8266WebServer(80);

  server->on("/", []() {
    content = contentHeader + ssidHeader;

    for (int i = 0; i < numberOfNetworks; ++i) {
      content += String("                <option value='") + networks[i] + "'" + ((networks[i] == ssid) ? " selected>" : ">") + networks[i] + "</option>\r\n";
    }

    content += String("          <tr style='background-color:#cccccc'><td align=right>Password:</td><td><input type=text id=pass name=pass value='") + pass + "'></td></tr>\r\n";
    content += ssidFooter;
    content += String("          <tr><td colspan=2 align=center><input type=submit id=submitButton name=submitButton value='Submit'></td></tr>\r\n");
    content += contentFooter;

    server->send(200, "text/html", content);
  });

  server->on("/setting", []() {
    ssid = server->arg("ssid");
    pass = server->arg("pass");
    ssid.replace( "+", " " );
    ssid.replace( "%40", "@" );
    if (ssid.length() > 0) {
        /*
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
            EEPROM.write(i, 0);
        }
        Serial.println(ssid);
        Serial.println("");
        Serial.println(pass);
        Serial.println("");

        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < ssid.length(); ++i)
        {
            EEPROM.write(i, ssid[i]);
            Serial.print("Wrote: ");
            Serial.println(ssid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < pass.length(); ++i)
        {
            EEPROM.write(32 + i, pass[i]);
            Serial.print("Wrote: ");
            Serial.println(pass[i]);
        }
        EEPROM.commit();
        */

        content = contentHeader + ssidHeader;
        for (int i = 0; i < numberOfNetworks; ++i) {
            content += String("                <option value='") + networks[i] + "'" + ((networks[i] == ssid) ? " selected>" : ">") + networks[i] + "</option>\r\n";
        }
        content += String("          <tr style='background-color:#cccccc'><td align=right>Password:</td><td><input type=text id=pass name=pass value='") + pass + "'></td></tr>\r\n";
        content += ssidFooter;
        content += String("          <tr><td colspan=2 align=center>Saved to EEPROM...<br>Reset to boot into new Wi-Fi.</td></tr>\r\n");
        content += contentFooter;

        server->send(200, "text/html", content);
        delay( 1000 );
        
        WiFi.begin(ssid.c_str(), pass.c_str());
        int c = 0;
        Serial.println("Waiting for Wifi to connect");
        while ( c < 15 ) {
            if (WiFi.status() == WL_CONNECTED) {
                Serial.println( "" );
                Serial.println("Connected.");
                Serial.println("");
                Serial.println("WiFi connected");
                Serial.print("Local IP: ");
                Serial.println(WiFi.localIP());
                Serial.print("SoftAP IP: ");
                break;
            }
            delay(1000);
            int n = WiFi.status();
            if( n == 0 )
            {
                Serial.println("No auto connect available");
                if( _boardType == ESPERT_BOARD_ESP201 ) {
                    digitalWrite( ESPERT_PIN_LED, LOW );
                }
                else {
                    digitalWrite( ESPERT_PIN_LED, HIGH );
                }
                break;
            } 
            Serial.print(n);
                if( _boardType == ESPERT_BOARD_ESP201 ) {
                    digitalWrite( ESPERT_PIN_LED, HIGH );
                }
                else {
                    digitalWrite( ESPERT_PIN_LED, LOW );
                }
            delay( 50 );
            if( _boardType == ESPERT_BOARD_ESP201 ) {
                digitalWrite( ESPERT_PIN_LED, LOW );
            }
            else {
                digitalWrite( ESPERT_PIN_LED, HIGH );
            }
            c++;
        }        
        
        ESP.reset();
    } else {
        content = "Error";
        Serial.println("Sending 404");
        server->send(200, "text/html", content);
    }
  });

  server->begin();
  Serial.println("HTTP server started");
}

void ESPert_WiFi::disconnect( bool reset )
{
    Serial.println("WiFi disconnected.");
    if( _boardType == ESPERT_BOARD_ESP201 ) {    
        _espert->EEPROM_Write( 237, "TESPA:Disconnect");
        Serial.println("WiFi disconnect on restart");
    }
    else {
        WiFi.disconnect();
        delay(100);
    }
    if( reset )
    {
        ESP.reset();
    }
}

void ESPert_WiFi::setAutoConnect( bool bAuto )
{
    Serial.print("WiFi set auto connect: ");
    Serial.println(bAuto);

    if( bAuto ) {
        _espert->EEPROM_Write( 237, "TESPA:AutoConnect");
        Serial.println("WiFi auto connect on restart");
    } 
    else {
        _espert->EEPROM_Write( 237, "TESPA:NoAutoConnect");
        Serial.println("WiFi NO auto connect on restart");
    }   
    delay(100);
    ESP.reset();
}


String ESPert_WiFi::getLocalIP()
{
    IPAddress ip =  WiFi.localIP();
    char textID[ 32 ];
    sprintf( textID, "%i.%i.%i.%i", ip[0], ip[1], ip[2], ip[3]);
    return String( textID );
}

String ESPert_WiFi::getAPIP()
{
    IPAddress ip =  WiFi.softAPIP();
    char textID[ 32 ];
    sprintf( textID, "%i.%i.%i.%i", ip[0], ip[1], ip[2], ip[3]);
    return String( textID );
}


// *************************************************
// EEPROM 
// *************************************************
String ESPert::EEPROM_Read( int index, int length ) 
{
  EEPROM.begin(512);
  delay(10);
  Serial.println("Reading EEPROM");
  String text = "";
  char ch = 1;
  for (int i = index; (i < (index+length)) && ch; ++i)
  {
    if (ch = EEPROM.read(i))
    {
        text.concat(ch);
        //Serial.print("Read: ");
        //Serial.println(ch);
    }
  }
  Serial.print("TEXT: ");
  Serial.println(String(text.c_str()));
  Serial.println("Reading EEPROM pass");
  return text;
}

int ESPert::EEPROM_Write( int index, String text ) 
{
  EEPROM.begin(512);
  delay(10);
  Serial.println("Writing EEPROM");
  Serial.println(text);
  for (int i = index; i<text.length()+index; ++i)
  {
    EEPROM.write( i, text[i-index] );
    //Serial.print("Wrote: ");
    //Serial.println(text[i-index]);
  }
  EEPROM.write( index+text.length(), 0 );
  EEPROM.commit();

  Serial.println("Writing EEPROM pass");
  return text.length()+1;
}

void ESPert_GroveRelay::on() {
    ESPert_LED::set(false);
}

void ESPert_GroveRelay::off() {
    ESPert_LED::set(true);
}

void ESPert_GroveRelay::set(bool state) {
    ESPert_LED::set( !state );
}

int ESPert_GroveRelay::get() {
    return !ESPert_LED::get();
}


void ESPert_MQTT::init( IPAddress server, int port )
{
    callback = NULL;
    IPAddress mqtt_server = server;
    mqtt_client = new PubSubClient(mqtt_server, port);
}

void ESPert_MQTT::init( String server, int port )
{
    callback = NULL;
    String mqtt_server = server;
    mqtt_client = new PubSubClient(mqtt_server, port);
}

PubSubClient *ESPert_MQTT::getPubSubClient()
{
    return mqtt_client;
}

String ESPert_MQTT::getClientName()
{
    String clientName;
    uint8_t mac[6];

    clientName += "ESPert-";
    WiFi.macAddress(mac);
    clientName += macToStr(mac);
    clientName += "-";
    clientName += String(micros() & 0xff, 16);  
    return clientName;
}

void ESPert_MQTT::publish( String topic, String value )
{
    // Publish temperature
    if( mqtt_client ) {
        mqtt_client->publish(topic, (char *) value.c_str());
    }
}

void ESPert_MQTT::subscribe( String topic )
{
    if( mqtt_client ) {
        _espert->println( "Subscribe: " + topic );
        mqtt_client->subscribe(topic);
    }
}

void ESPert_MQTT::setCallback(PubSubClient::callback_t cb) {
    callback = cb;
}

/* MQTT server connection */
void ESPert_MQTT::connect() {
  // add reconnection logics
  if (!mqtt_client->connected()) {
    mqtt_client->set_callback(callback);

    // connection to MQTT server
    String cn = getClientName();
    if (mqtt_client->connect((char *)cn.c_str())) 
    {
      Serial.println("[PHYSICAL] Successfully connected with MQTT");
      //Serial.print( "Server: " );
      //Serial.println( mqtt_client->server_hostname );
      Serial.print( "Client ID: " );
      Serial.println( cn );
    }
  }
  mqtt_client->loop();
}

ESPert_SoftwareSerial::ESPert_SoftwareSerial()
{
    swSerial = NULL;
}

void ESPert_SoftwareSerial::init( int rx, int tx, int buffer )
{
    swSerial = new SoftwareSerial( rx, tx, buffer );
    

}

String ESPert_SoftwareSerial::readString()
{
  _readString = "";
  while (swSerial && swSerial->available()) {
    char c = swSerial->read();  //gets one byte from serial buffer
    _readString += c; //makes the string _readString
    delay(5);  //slow looping to allow buffer to fill with next character
  }
  if( _readString.length() > 0 )
    return _readString;
  return String("");
}
/*

void ESPert_SoftwareSerial::write( String str ) {
  for( int i=0; i<str.length(); i++ ) {
    swSerial->write(str[i]);
  }
}
*/
size_t ESPert_SoftwareSerial::write(uint8_t b) {
    swSerial->write( b );
}

int ESPert_SoftwareSerial::read()
{
    return swSerial->read();
}

int ESPert_SoftwareSerial::available()
{
    return swSerial->available();
}

void ESPert_SoftwareSerial::flush()
{
    swSerial->flush();
}

int ESPert_SoftwareSerial::peek()
{
    swSerial->peek();
}

void ESPert_SoftwareSerial::begin( int baud )
{
    swSerial->begin( baud);
    
    for (char ch = ' '; ch <= 'z'; ch++) {
        swSerial->write(ch);
    }
    swSerial->println("");

    _readString = "";
}



bool ESPert_BLE::init( ESPert_SoftwareSerial *swSer )
{
  swSerial = swSer;
    
  swSerial->write( "AT" );
  delay( 100 );

  _readString = swSerial->readString();
  _espert->println( _readString );
  _espert->println( "***" );
  if( _readString == "OK" )
  {
    delay( 100 );
    return true;
  }
  return false;
}

String ESPert_BLE::getFirmwareVersion()
{
  swSerial->write( "AT+VERR?" );
  delay( 100 );
  _readString = swSerial->readString();
  if( _readString.length() != 0 )
  {
    return _readString.substring(7);
  }
  return "(unknown)";
}

bool ESPert_BLE::isOn()
{
  swSerial->write( "AT+IBEA?" );
  delay( 100 );
  _readString = swSerial->readString();
  if( _readString.length() != 0 )
  {
    _espert->println( _readString );
    if( _readString == "OK+Get:1" ) {
      delay( 100 );
      return true;
    }
    else if( _readString == "OK+Get:0" ){
      delay( 100 );
      return false;
    }
  }
  _espert->println( "Unknown command\n" );  
  return false;

}

bool ESPert_BLE::on()
{
  swSerial->write( "AT+IBEA1" );
  delay( 100 );
  _readString = swSerial->readString();
  if( _readString.length() != 0 )
  {
    _espert->println( _readString );
    if( _readString == "OK+Set:1" ) {
      delay( 100 );
      return true;
    }
    else if( _readString == "OK+Set:0" ){
      delay( 100 );
      return false;
    }
  }
  _espert->println( "Unknown command\n" );  
  return false;
}

bool ESPert_BLE::off()
{
  swSerial->write( "AT+IBEA0" );
  delay( 100 );
  _readString = swSerial->readString();
  if( _readString.length() != 0 )
  {
    _espert->println( _readString );
    if( _readString == "OK+Set:0" ) {
      delay( 100 );
      return true;
    }
    else if( _readString == "OK+Set:1" ){
      delay( 100 );
      return false;
    }
  }
  _espert->println( "Unknown command\n" );  
  return false;
}


String ESPert_BLE::getUUID()
{
  String UUID = "";
  swSerial->write( "AT+IBE0?" );
  delay( 100 );
  _readString = swSerial->readString();
  if( _readString.length() != 0 )
  {
    _espert->println( _readString.substring( 7 ) );
    UUID += _readString.substring( 9 );
  }
  swSerial->write( "AT+IBE1?" );
  delay( 100 );
  _readString = swSerial->readString();
  if( _readString.length() != 0 )
  {
    _espert->println( _readString.substring( 7 ) );
    UUID += _readString.substring( 9 );
  }
  swSerial->write( "AT+IBE2?" );
  delay( 100 );
  _readString = swSerial->readString();
  if( _readString.length() != 0 )
  {
    _espert->println( _readString.substring( 7 ) );
    UUID += _readString.substring( 9 );
  }
  swSerial->write( "AT+IBE3?" );
  delay( 100 );
  _readString = swSerial->readString();
  if( _readString.length() != 0 )
  {
    _espert->println( _readString.substring( 7 ) );
    UUID += _readString.substring( 9 );
  }

  
  return UUID;
}


int ESPert_BLE::getMajor()
{
  swSerial->write( "AT+MARJ?" );
  delay( 100 );
  _readString = swSerial->readString();
  if( _readString.length() != 0 )
  {
    //_espert->println( _readString );
    int l = strtol( _readString.substring(9).c_str(), NULL, 16 );
    return l;
  }
  return -1;
}

int ESPert_BLE::getMinor()
{
  swSerial->write( "AT+MINO?" );
  delay( 100 );
  _readString = swSerial->readString();
  if( _readString.length() != 0 )
  {
    //_espert->println( _readString );
    int l = strtol( _readString.substring(9).c_str(), NULL, 16 );
    return l;
  }
  return -1;
}

int ESPert_BLE::getTXPower()
{
  swSerial->write( "AT+MEAS?" );
  delay( 100 );
  _readString = swSerial->readString();
  if( _readString.length() != 0 )
  {
    //_espert->println( _readString );
    int l = strtol( _readString.substring(9).c_str(), NULL, 16 );
    return l;
  }
  return -1;
}

