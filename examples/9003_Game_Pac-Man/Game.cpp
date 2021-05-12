#include "Game.h"

Game::Game() {
  gameIndex = GAME_UNKNOWN;
  isRequestingExit = false;
  resetGameTime();

  // time
  hours = 0;
  minutes = 0;
  seconds = 0;
  secondsChanged = false;
  timeSyncLastFrameTime = lastFrameTime;
  isSyncInternetTime = false;

  // frame rate
  frameRate = 0;
  frameCount = 0l;
  memset(&fpsDigit, 0, sizeof(fpsDigit));

  // button
  isGamepadEnabled = false;
  buttonDelay = 0.0f;
  isButtonPressed[numberOfButtons] = {false};
  pressedButton = BUTTON_NONE;
  isButtonAllowed = true;
  memset(buttonPin, -1, sizeof(buttonPin));

#ifdef ARDUINO_ESP8266_ESPRESSO_LITE_V1
  buttonPin[0] = 0; // USER
  buttonPin[1] = 2; // FLASH
#else
  buttonPin[0] = 0; // GPIO0
  buttonPin[1] = 13; // GPIO13
#endif

  // sound
  soundDuration = 0.0f;
  nextSound = 0;
  nextSoundDelay = 0.0f;
  volume = 1.0f; // 0.0 to 1.0
  isVolumeChanged = 0.0f;
  isSoundInterruptEnabled = true;
  isSoundEnabled = true;
  readVolume();

  // battery
  battery = 0.0f; // 0.0 to 1.0
  batteryVoltage = 0;
  lastBatteryVoltage = -1;
  batteryA0Value = batteryA0Min;

  // score
  highScore = 0l;
  score = 0l;
  highScoreAddress = -1;
}

void Game::decreaseVolume() {
  volume = constrain(volume - (elapsedTime / 1000.0f), 0.0f, 1.0f);
  isVolumeChanged = 1000.0f;
}

void Game::drawBitmap(int x, int y, int width, int height, const uint8_t* bitmap, const uint8_t* mask, int color) {
  if (mask) {
    espert->oled.setColor(1 - color);
    espert->oled.drawBitmap(x, y, width, height, mask, false);
  }

  espert->oled.setColor(color);
  espert->oled.drawBitmap(x, y, width, height, bitmap, false);
}

String Game::floatToString(float value, int length, int decimalPalces) {
  String stringValue = String(value, decimalPalces);
  String prefix = "";

  for (int i = 0; i < length - stringValue.length(); i++) {
    prefix += " ";
  }

  return prefix + stringValue;
}

Game::GameIndex Game::getGameIndex() {
  return gameIndex;
}

int Game::getBatteryVoltage() {
  batteryA0Value = analogRead(A0);

  if (batteryA0Value > batteryA0Min) {
    batteryFilters[batteryFiltersIndex++ % batteryMaxValues] = batteryA0Value;
    batteryA0Value = median(batteryFilters, batteryMaxValues);
    batteryVoltage = map(batteryA0Value, 0, 1023, 0, batteryMaxVoltage);
  }

  return batteryVoltage;
}

int Game::getHighScoreAddress() {
  int address = -1;

  if (gameIndex != GAME_UNKNOWN) {
    address = saveDataHeaderSize;

    for (int i = numberOfGames - 1; i > gameIndex; i--) {
      int length = String(maxScore[i]).length() + 1;

      if (i == GAME_OCTOPUS) {
        address += length * 2; // game A + game B
      } else {
        address += length;
      }
    }
  }

  return address;
}

void Game::increaseVolume() {
  volume = constrain(volume + (elapsedTime / 1000.0f), 0.0f, 1.0f);
  isVolumeChanged = 1000.0f;
}

void Game::initBattery() {
  for (int i = 0; i < batteryMaxValues; ++i) {
    getBatteryVoltage();
  }
}

void Game::initGame() {
}

void Game::init(ESPert* e, bool menu, bool syncInternetTime, int hh, int mm, int ss) {
  isMenuEnabled = menu;
  isSyncInternetTime = syncInternetTime;
  hours = hh;
  minutes = mm;
  seconds = ss;

  isGamepadEnabled = (analogRead(A0) > batteryA0Min) ? true : false;

  espert = e;
  espert->oled.init();
  espert->buzzer.init(isGamepadEnabled ? 15 : 12);
  espert->buzzer.on(1);
  espert->buzzer.on(0);

  int gamepadPin[numberOfButtons] = {12, 13, 14, 2, 0, A0}; // (left, right, up, down, a, b)
  for (int i = 0; i < numberOfButtons; i++) {
    if (isGamepadEnabled) {
      buttonPin[i] = gamepadPin[i];
    }

    if (buttonPin[i] != -1) {
      button[i].init(buttonPin[i], INPUT_PULLUP);
    }
  }

  randomSeed(analogRead(0));
  initGame();
  resetGameTime();
  timeSyncLastFrameTime = lastFrameTime;
  readInternetTime();

  initBattery();
}

bool Game::isBackToMenuEnabled() {
  return false;
}

bool Game::isBlink(float factor) {
  int speed = maxBlinkTime * factor;
  return ((int)blinkTime % speed < speed * 0.5f) ? true : false;
}

bool Game::isExit() {
  return isRequestingExit;
}

bool Game::isSecondChanged() {
  bool flag = secondsChanged;
  secondsChanged = false;
  return flag;
}

float Game::lerp(float t, float v0, float v1) {
  v0 = (1.0f - t) * v0 + t * v1;

  if (fabs(v1 - v0) < 1.0f) {
    v0 = v1;
  }

  return v0;
}

String Game::longToString(unsigned long value, int length, String prefixChar) {
  String stringValue = String(value);
  String prefix = "";

  for (int i = 0; i < length - stringValue.length(); i++) {
    prefix += prefixChar;
  }

  return prefix + stringValue;
}

int Game::median(int arr[], int maxValues) {
  quickSort(arr, 0, maxValues - 1);
  return arr[maxValues / 2];
}

int Game::partition(int* arr, const int left, const int right) {
  const int mid = left + (right - left) / 2;
  const int pivot = arr[mid];

  // move the mid point value to the front.
  swap(arr[mid], arr[left]);
  int i = left + 1;
  int j = right;

  while (i <= j) {
    while (i <= j && arr[i] <= pivot) {
      i++;
    }

    while (i <= j && arr[j] > pivot) {
      j--;
    }

    if (i < j) {
      swap(arr[i], arr[j]);
    }
  }

  swap(arr[i - 1], arr[left]);
  return i - 1;
}

void Game::playSound(int index) {
}

void Game::pressButton() {
}

void Game::quickSort(int* arr, const int left, const int right) {
  if (left >= right) {
    return;
  }

  int part = partition(arr, left, right);

  quickSort(arr, left, part - 1);
  quickSort(arr, part + 1, right);
}

unsigned long Game::readHighScore(int offset) {
  if (gameIndex != GAME_UNKNOWN) {
    if (espert->eeprom.read(0, saveDataKey.length()) == saveDataKey) {
      highScoreAddress = getHighScoreAddress();

      if (highScoreAddress != -1) {
        String data = espert->eeprom.read(highScoreAddress + (offset > 0 ? 1 : 0) + offset, String(maxScore[gameIndex]).length());
        highScore = data.toInt();
        if (highScore < 0l || highScore > maxScore[gameIndex]) {
          highScore = constrain(highScore, 0, maxScore[gameIndex]);
          writeHighScore(offset);
        }
      }
    } else {
      for (int i = 0; i < 512; i++) {
        EEPROM.write(i, 0);
      }

      espert->eeprom.write(0, saveDataKey);
      highScore = 0l;
      writeHighScore(offset);
    }
  }

  return highScore;
}

bool Game::readInternetTime() {
  bool success = false;

  if (isSyncInternetTime && wifiSSID && strlen(wifiSSID) > 0) {
    String internetTime = "";
    espert->print("Connecting to WiFi ");
    espert->print(wifiSSID);
    WiFi.begin(wifiSSID, wifiPassword);

    int retry = 20;
    while (WiFi.status() != WL_CONNECTED && --retry > 0) {
      espert->print(".");
      delay(500);
    }

    espert->println();

    if (retry > 0) {
      espert->print("Connected, IP address: ");
      espert->println(WiFi.localIP());

      retry = 3;
      WiFiClient client;
      espert->println("Connecting to google.com to read current time...");
      while (!!!client.connect("google.com", 80) && --retry > 0) {
        espert->println("Retrying...");
      }

      if (retry > 0) {
        client.print("HEAD / HTTP/1.1\r\n\r\n");

        while (!!!client.available()) {
          yield();
        }

        while (client.available()) {
          if (client.read() == '\n') {
            if (client.read() == 'D') {
              if (client.read() == 'a') {
                if (client.read() == 't') {
                  if (client.read() == 'e') {
                    if (client.read() == ':') {
                      client.read();
                      internetTime = client.readStringUntil('\r');
                    }
                  }
                }
              }
            }
          }
        }
      } else {
        espert->println("Failed!");
      }

      client.stop();
      espert->println(internetTime); // ddd, dd mmm yyyy hh:mm:ss GMT
    } else {
      espert->println("Failed!");
    }

    WiFi.disconnect();
    success = true;

    if (internetTime.length() > 0) {
      String hh = internetTime.substring(17, 19);
      String mm = internetTime.substring(20, 22);
      String ss = internetTime.substring(23, 25);

      int h = (int)timeZone;
      int m = round((timeZone - h) * 100.0f);
      hours = hh.toInt() + h;
      minutes = mm.toInt() + m;

      if (minutes < 0) {
        minutes += 60;
        hours--;
      } else if (minutes >= 60) {
        minutes -= 60;
        hours++;
      }

      if (hours < 0) {
        hours += 24;
      } else if (hours >= 24) {
        hours -= 24;
      }

      seconds = ss.toInt();
    }
  }

  return success;
}

void Game::readVolume() {
  if (espert->eeprom.read(0, saveDataKey.length()) == saveDataKey) {
    String data = espert->eeprom.read(saveDataKey.length() + 1, saveDataVolumeLength);
    volume = data.toFloat();
    if (volume < 0.0f || volume > 1.0f) {
      volume = constrain(volume, 0.0f, 1.0f);
      writeVolume();
    }
  } else {
    for (int i = 0; i < 512; i++) {
      EEPROM.write(i, 0);
    }

    espert->eeprom.write(0, saveDataKey);
    volume = 1.0f;
    writeVolume();
  }
}

void Game::render() {
}

void Game::renderBattery(int x, int y, int color, int bitmapWidth, int bitmapHeight, int gap, const uint8_t* numberBitmap, const uint8_t* numberMaskBitmap) {
  if (isGamepadEnabled) {
    if (getBatteryVoltage() != lastBatteryVoltage) {
      lastBatteryVoltage = batteryVoltage;
      battery = constrain(((float)batteryVoltage - batteryVoltageMin) / batteryVoltageLength, 0.0f, 1.0f);
      espert->println(String(millis()) + ": Battery Voltage = " + String(batteryVoltage));
    }

    if (numberBitmap != NULL) {
      String string = String(batteryVoltage);
      int width = bitmapWidth + gap;
      int offset = x - 1 - (string.length() * width);

      for (int i = 0; i < string.length(); i++) {
        int n = string.charAt(i) - '0';
        drawBitmap(offset + (i * width), y, bitmapWidth, bitmapHeight, numberBitmap + (n * bitmapWidth), numberMaskBitmap + (n * bitmapWidth), ESPERT_BLACK);
      }
    }

    drawBitmap(x, y, 16, 8, batteryBitmap, NULL, color);

    for (int i = 0; i < (int)round(battery * 10.0f); i++) {
      if (battery > 0.2f || (battery <= 0.2f && isBlink())) {
        drawBitmap(x + 2 + i, y + 2, 8, 8, batteryIndicatorBitmap, NULL, color);
      }
    }
  }
}

void Game::renderFPS(int x, int y, int bitmapWidth, int bitmapHeight, int gap, const uint8_t* numberBitmap, const uint8_t* numberMaskBitmap, int color, int fpsBitmapX, int fpsBitmapY) {
  if (isFPSVisibled && numberBitmap) {
    if (fpsBitmapX > -1 && fpsBitmapY > -1) {
      drawBitmap(fpsBitmapX, fpsBitmapY, 16, 8, fpsBitmap, fpsMaskBitmap, color);
    }

    for (int i = 0; i < 2 && (fpsDigit[i] >= 0 && fpsDigit[i] <= 9); i++) {
      drawBitmap(x + (i * (bitmapWidth + gap)), y, bitmapWidth, bitmapHeight, numberBitmap + (fpsDigit[i] * bitmapWidth), numberMaskBitmap ? numberMaskBitmap + (fpsDigit[i] * bitmapWidth) : NULL, color);
    }
  }
}

void Game::renderVolume(int x, int y, int color) {
  if (isSoundEnabled) {
    int i = (int)round(volume * (numberOfVolumeFrames - 1));

    if (i == 0 && volume > 0.0f) {
      i = 1;
    }

    drawBitmap(x, y, 16, 8, volumeBitmap[i], NULL, color);
  }
}

void Game::renderMakerAsia(int x, int y, int color) {
  drawBitmap(x, y, 64, 16, makerAsiaBitmap, makerAsiaMaskBitmap, color);
}

void Game::update() {
}

void Game::resetGameTime() {
  lastFrameTime = millis();
  elapsedTime = 0.0f;
  blinkTime = 0.0f;
  fpsLastFrameTime = lastFrameTime;
}

void Game::swap(int &a, int &b) {
  int t = a;
  a = b;
  b = t;
}

void Game::toggleVolume() {
  if (volume == 0.0f) {
    volume = 1.0f;
  } else {
    volume = 0.0f;
  }

  isVolumeChanged = 1000.0f;
}

void Game::updateGameTime(bool updateButton) {
  // game time
  unsigned long frameTime = millis();
  elapsedTime = frameTime - lastFrameTime;
  lastFrameTime = frameTime;

  updateInternetTime();

  // frame rate
  frameCount++;
  if (frameTime - fpsLastFrameTime >= 1000l) {
    if (frameRate != frameCount) {
      frameRate = frameCount;
      String fpsString = longToString(constrain(frameRate, 0, 99), 2, "0");

      for (int i = 0; i < 2; i++) {
        fpsDigit[i] = fpsString.charAt(i) - '0';
      }
    }

    frameCount = 0l;
    fpsLastFrameTime = frameTime;
  }

  // blink
  blinkTime += elapsedTime;
  if (blinkTime >= maxBlinkTime) {
    blinkTime = 0.0f;
  }

  // button
  if (updateButton) {
    buttonDelay += elapsedTime;
    if (buttonDelay >= maxButtonDelay) {
      buttonDelay = 0.0f;
      pressButton();
    }
  }

  // sound
  if (isSoundEnabled) {
    if (soundDuration > 0.0f) {
      soundDuration -= elapsedTime;
      if (soundDuration <= 0.0f) {
        soundDuration = 0.0f;
        espert->buzzer.off();
        isSoundInterruptEnabled = true;
      }
    } else if (nextSoundDelay > 0.0f) {
      nextSoundDelay -= elapsedTime;
      if (nextSoundDelay <= 0.0f) {
        nextSoundDelay = 0.0f;
        playSound(nextSound);
      }
    }
  }

  // volume
  if (isVolumeChanged > 0.0f) {
    isVolumeChanged -= elapsedTime;
    if (isVolumeChanged <= 0.0f) {
      isVolumeChanged = 0.0f;
      writeVolume();
    }
  }
}

void Game::updateInternetTime() {
  unsigned long t = millis() - timeSyncLastFrameTime;
  if (t >= 1000l) {
    timeSyncLastFrameTime = millis() - (t - 1000l);
    secondsChanged = true;

    if (++seconds > 59) {
      seconds = 0;

      if (++minutes > 59) {
        minutes = 0;

        if (++hours > 23) {
          hours = 0;
        }
      }
    }
  }
}

void Game::writeHighScore(int offset) {
  if (gameIndex != GAME_UNKNOWN) {
    highScoreAddress = getHighScoreAddress();

    if (highScoreAddress != -1) {
      espert->eeprom.write(highScoreAddress + (offset > 0 ? 1 : 0) + offset, longToString(highScore, String(maxScore[gameIndex]).length(), "0"));
    }
  }
}

void Game::writeVolume() {
  espert->eeprom.write(saveDataKey.length() + 1, floatToString(volume, saveDataVolumeLength, 2));
  isVolumeChanged = 0.0f;
}
