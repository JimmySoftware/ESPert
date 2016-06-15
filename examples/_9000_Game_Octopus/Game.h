#ifndef _GAME_H_
#define _GAME_H_

#include <ESPert.h>

static const int numberOfGames = 4;

// Convert Image to XBM http://www.online-utility.org/image/convert/to/XBM
static const uint8_t batteryBitmap[(16 * 8) / 8] PROGMEM = { // battery.png
  0xFF, 0x3F, 0x01, 0x20, 0x01, 0x60, 0x01, 0x60, 0x01, 0x60, 0x01, 0x60, 0x01, 0x20, 0xFF, 0x3F
};

static const uint8_t batteryIndicatorBitmap[(8 * 8) / 8] PROGMEM = { // batteryIndicator.png
  0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t fpsBitmap[(16 * 8) / 8] PROGMEM = { // fps.png
  0x00, 0x00, 0xEE, 0x0E, 0xA2, 0x02, 0xE6, 0x0E, 0x22, 0x08, 0x22, 0x0E, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t fpsMaskBitmap[(16 * 8) / 8] PROGMEM = { // fpsMask.png
  0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0xFF, 0x3F, 0x00, 0x00
};

static const uint8_t highScoreBitmap[(32 * 8) / 8] PROGMEM = { // highScore.png
  0x00, 0x00, 0x00, 0x00, 0x2A, 0xEE, 0xEE, 0x0E, 0x2A, 0x22, 0xAA, 0x02, 0xAE, 0x2E, 0x6A, 0x0E, 0x2A, 0x28, 0xAA, 0x02, 0x2A, 0xEE, 0xAE, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t highScoreMaskBitmap[(32 * 8) / 8] PROGMEM = { // highScoreMask.png
  0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t makerAsiaBitmap[(64 * 16) / 8] PROGMEM = { // makerAsia.png
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xB6, 0xD9, 0xAE, 0xEE, 0xEE, 0x3A, 0x00, 0x00, 0xCE, 0xA9, 0xAA, 0xA2,
  0x2A, 0x2A, 0x00, 0x00, 0xCE, 0xA9, 0x6E, 0x6E, 0xEE, 0x3A, 0x00, 0x00,
  0xB6, 0xA9, 0xAA, 0xA2, 0x8A, 0x2A, 0x00, 0x00, 0xFC, 0x88, 0xAA, 0xAE,
  0xEA, 0x2A, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t makerAsiaMaskBitmap[(64 * 16) / 8] PROGMEM = { // makerAsiaMask.png
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xFE, 0xF9, 0xAE, 0xFE, 0xFF, 0x3A, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xFE, 0xFD, 0xFF, 0xFF,
  0xFF, 0x7F, 0x00, 0x00, 0xFC, 0x88, 0xAA, 0xBE, 0xEB, 0x6A, 0x00, 0x00,
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const int numberOfVolumeFrames = 4;
static const uint8_t volumeBitmap[numberOfVolumeFrames][(16 * 8) / 8] PROGMEM = {
  {0x00, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0C, 0x00, 0x08, 0x00, 0x00, 0x00}, // volume0.png
  {0x00, 0x00, 0x08, 0x00, 0x2C, 0x00, 0x4F, 0x00, 0x4F, 0x00, 0x2C, 0x00, 0x08, 0x00, 0x00, 0x00}, // volume1.png
  {0x00, 0x00, 0x88, 0x00, 0x2C, 0x01, 0x4F, 0x01, 0x4F, 0x01, 0x2C, 0x01, 0x88, 0x00, 0x00, 0x00}, // volume2.png
  {0x00, 0x02, 0x88, 0x04, 0x2C, 0x05, 0x4F, 0x05, 0x4F, 0x05, 0x2C, 0x05, 0x88, 0x04, 0x00, 0x02}  // volume3.png
};

class Game {
  public:
    // ****************************************************************************
    // EEPROM
    // ****************************************************************************i
    //               saveDataHeaderSize (32)           highScore
    //          ┌───────────────┴──────────────┐┌──────────┴──────────┐
    // Address: 0123456789012345678901234567890123456789012345678901234567890123456
    // Data:    ESPressoGames 1.00              999 999 9999 999 999999
    //          │             │                 │   │   │    │   └─ GAME_PAC_MAN
    //          │             │                 │   │   │    └─ GAME_FLAPPY_BIRD
    //          │             │                 │   │   └─ GAME_BREAKOUT
    //          │             │                 │   └─ GAME_OCTOPUS (Game B)
    //          │             │                 └─ GAME_OCTOPUS (Game A)
    //          │             └─ volume (0.00 to 1.00)
    //          └─ saveDataKey
    // ****************************************************************************
    typedef enum {
      GAME_UNKNOWN = -1,
      GAME_PAC_MAN,
      GAME_FLAPPY_BIRD,
      GAME_BREAKOUT,
      GAME_OCTOPUS,
    } GameIndex;
    GameIndex gameIndex;
    static const int saveDataHeaderSize = 32;
    const String saveDataKey = "ESPressoGames";
    static const int saveDataVolumeLength = 4; // 0.00 to 1.00
    const unsigned long maxScore[numberOfGames] = {999999l, 999l, 9999l, 999l}; // GAME_PAC_MAN, ..., GAME_OCTOPUS

    // internet time
    const char* wifiSSID = ""; // your WiFI network name
    const char* wifiPassword = ""; // your WiFi password
    const float timeZone = +7.00f; // your time zone (+7.00 is UTC+07:00 Bangkok)
    bool isSyncInternetTime;
    unsigned long timeSyncLastFrameTime;
    int hours;
    int minutes;
    int seconds;
    bool secondsChanged;

    typedef struct SizeStruct {
      int width;
      int height;
    } Size;

    typedef struct PointStruct {
      float x;
      float y;
    } Point;

    typedef enum {
      BUTTON_NONE = -1,
      BUTTON_LEFT,
      BUTTON_RIGHT,
      BUTTON_UP,
      BUTTON_DOWN,
      BUTTON_A,
      BUTTON_B
    } ButtonType;

    const Size screenSize = {128, 64};
    bool isRequestingExit;
    static const int maxBlinkTime = 500;
    bool isMenuEnabled;

    // game time
    unsigned long lastFrameTime;
    float elapsedTime;
    float blinkTime;

    // frame rate
    unsigned long frameRate;
    unsigned long frameCount;
    unsigned long fpsLastFrameTime;
    const bool isFPSVisibled = false;
    int fpsDigit[2];

    // button
    static const int numberOfButtons = 6;
    static const int maxButtonDelay = 10;
    int buttonPin[numberOfButtons];
    ESPert_Button button[numberOfButtons];
    bool isGamepadEnabled;
    float buttonDelay = 0.0f;
    bool isButtonPressed[numberOfButtons];
    int pressedButton;
    bool isButtonAllowed;

    // sound
    const Size volumeSize = {13, 11};
    float soundDuration;
    int nextSound;
    float nextSoundDelay;
    float volume;
    float isVolumeChanged;
    bool isSoundInterruptEnabled;
    bool isSoundEnabled = true;

    // battery
    static const int batteryA0Min = 5;
    static const int batteryMaxVoltage = 6080;
    static const int batteryVoltageMin = 3000;
    static const int batteryVoltageMax = 4225;
    static const int batteryVoltageLength = batteryVoltageMax - batteryVoltageMin + 1;
    static const int batteryMaxValues = 21;
    int batteryFilters[batteryMaxValues];
    int batteryFiltersIndex = 0;
    int batteryA0Value;
    int lastBatteryVoltage;
    int batteryVoltage;
    const Size batterySize = {15, 8};
    float battery;

    // score
    unsigned long highScore;
    unsigned long score;
    int highScoreAddress;

    ESPert* espert;

    void decreaseVolume();
    void drawBitmap(int x, int y, int width, int height, const uint8_t* bitmap, const uint8_t* mask = NULL, int color = ESPERT_WHITE);
    String floatToString(float value, int length, int decimalPalces);
    int getBatteryVoltage();
    int getHighScoreAddress();
    void increaseVolume();
    void initBattery();
    virtual void initGame();
    bool isBlink(float factor = 1.0f);
    bool isSecondChanged();
    float lerp(float t, float v0, float v1);
    String longToString(unsigned long value, int length, String prefixChar = "0");
    int median(int arr[], int maxValues);
    int partition(int* arr, const int left, const int right);
    virtual void playSound(int index);
    virtual void pressButton();
    void quickSort(int* arr, const int left, const int right);
    unsigned long readHighScore(int offset = 0);
    bool readInternetTime();
    void readVolume();
    void renderBattery(int x, int y, int color = ESPERT_WHITE, int bitmapWidth = 0, int bitmapHeight = 0, int gap = 0, const uint8_t* numberBitmap = NULL, const uint8_t* numberMaskBitmap = NULL);
    void renderVolume(int x, int y, int color = ESPERT_WHITE);
    void renderFPS(int x, int y, int bitmapWidth, int bitmapHeight, int gap, const uint8_t* numberBitmap, const uint8_t *numberMaskBitmap = NULL, int color = ESPERT_WHITE, int fpsBitmapX = -1, int fpsBitmapY = -1);
    void renderMakerAsia(int x, int y, int color = ESPERT_WHITE);
    void resetGameTime();
    void swap(int &a, int &b);
    void toggleVolume();
    void updateGameTime(bool updateButton = true);
    void updateInternetTime();
    void writeHighScore(int offset = 0);
    void writeVolume();

  public:
    Game();
    Game::GameIndex getGameIndex();
    void init(ESPert* e, bool menu = false, bool syncInternetTime = false, int hh = 0, int mm = 0, int ss = 0);
    virtual bool isBackToMenuEnabled();
    bool isExit();
    virtual void update();
    virtual void render();
};

#endif
