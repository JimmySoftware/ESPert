#include "Menu.h"

using namespace menu;

Menu::Menu() {
}

void Menu::initGame() {
  game = NULL;
  currentGame = 0;
  targetGame = currentGame;
  offset = 0.0f;
  targetOffset = 0.0f;
  arrowTime = 0.0f;
  arrowOffset = 0.0f;
  backToMenu = true;
}

void Menu::moveLeft() {
  if (targetOffset == 0.0f) {
    targetGame = currentGame + 1;
    if (targetGame >= numberOfGames) {
      targetGame = 0;
    }
    targetOffset = -screenSize.width;
  }
}

void Menu::moveRight() {
  if (targetOffset == 0.0f) {
    targetGame = currentGame - 1;
    if (targetGame < 0) {
      targetGame = numberOfGames - 1;
    }
    targetOffset = screenSize.width;
  }
}

void Menu::playSound(int index) {
  if (isSoundEnabled) {
    nextSound = SOUND_NONE;
    nextSoundDelay = 0.0f;
    int frequency = 0;

    switch (index) {
      case SOUND_VOLUME:
        frequency = 25;
        soundDuration = 100.0f;
        break;

      default:
        frequency = 0;
        soundDuration = 0.0f;
        break;
    }

    espert->buzzer.on((int)constrain(frequency * volume, 0.0f, 25.0f));
  }
}

void Menu::pressButton() {
  if (!game) {
    for (int i = 0; i < numberOfButtons; i++) {
      if (buttonPin[i] != -1) {
        bool isPressed = false;

        if (buttonPin[i] == A0) {
          isPressed = (batteryA0Value >= batteryA0Min) ? false : true;
        } else {
          isPressed = (digitalRead(buttonPin[i]) == LOW) ? true : false;
        }

        if (isPressed != isButtonPressed[i]) {
          isButtonPressed[i] = isPressed;

          if (isPressed) {
            pressedButton = i;
          } else {
            if (isVolumeChanged > 0.0f) {
              if (isGamepadEnabled && (i == BUTTON_UP || i == BUTTON_DOWN)) {
                playSound(SOUND_VOLUME);
              }
            } else if (i == pressedButton && targetOffset == 0.0f) {
              if (i == BUTTON_A) {
                switch (currentGame) {
                  case GAME_PAC_MAN:
                    game = new pacMan::PacMan();
                    break;

                  case GAME_FLAPPY_BIRD:
                    game = new flappyBird::FlappyBird();
                    break;

                  case GAME_BREAKOUT:
                    game = new breakout::Breakout();
                    break;

                  case GAME_OCTOPUS:
                    game = new octopus::Octopus();
                    break;
                }

                if (game) {
                  bool hasMenu = true;
                  bool syncInternetTime = false;
                  game->init(espert, hasMenu, syncInternetTime, hours, minutes, seconds);
                  backToMenu = false;
                }
              }
            }

            pressedButton = BUTTON_NONE;
          }

          break;
        }
      }
    }
  }
}

void Menu::render() {
  espert->oled.clear(false);

  if (game) {
    game->render();
  } else {
    espert->oled.setColor(ESPERT_BLACK);
    drawBitmap(0, 0, screenSize.width, screenSize.height, backgroundBitmap, NULL, ESPERT_WHITE);
    renderMakerAsia(40, 54, ESPERT_BLACK);
    drawBitmap(offset, 10, 128, 40, gameBitmap[currentGame], NULL, ESPERT_BLACK);

    if (targetOffset != 0.0f) {
      drawBitmap(offset - targetOffset, 10, 128, 40, gameBitmap[targetGame], NULL, ESPERT_BLACK);
    }

    if (targetOffset <= 0.0f) {
      drawBitmap(screenSize.width - 13 - arrowOffset, 44, 16, 16, rightArrowBitmap, rightArrowMaskBitmap, ESPERT_BLACK);
    }

    drawBitmap(58, 47, 16, 8, ofBitmap, ofMaskBitmap, ESPERT_BLACK);
    int length = String(numberOfGames).length();
    String string = String(targetGame + 1);
    int n = 0;

    for (int i = 0; i < length; i++) {
      n = string.charAt(i) - '0';
      drawBitmap(49 - ((length - 1 - i) * 5), 47, 8, 8, numberBitmap[n], numberMaskBitmap[n], ESPERT_BLACK);
    }

    string = String(numberOfGames);
    for (int i = 0; i < string.length(); i++) {
      n = string.charAt(i) - '0';
      drawBitmap(72 + (i * 5), 47, 8, 8, numberBitmap[n], numberMaskBitmap[n], ESPERT_BLACK);
    }

    renderVolume(1, 1, ESPERT_BLACK);
    //renderBattery(screenSize.width - batterySize.width - 1, 1, ESPERT_BLACK, 8, 8, -3, (const uint8_t*)numberBitmap, (const uint8_t*)numberMaskBitmap); // show battery voltage
    renderBattery(screenSize.width - batterySize.width - 1, 1, ESPERT_BLACK);
    renderFPS(116, 56, 8, 8, -3, (const uint8_t*)numberBitmap, (const uint8_t*)numberMaskBitmap, ESPERT_BLACK, 102, 56);
  }

  if (!game || backToMenu) {
    if (targetOffset >= 0.0f) {
      int color = ESPERT_BLACK;
      int y = 44;

      if (game) {
        if (game->getGameIndex() == GAME_BREAKOUT) {
          color = ESPERT_WHITE;
        } else if (game->getGameIndex() == GAME_OCTOPUS) {
          color = ESPERT_WHITE;
          y = 52;
        }

        drawBitmap(arrowOffset, y, 32, 16, menuBitmap, menuMaskBitmap, color);
      } else {
        drawBitmap(arrowOffset, y, 16, 16, leftArrowBitmap, leftArrowMaskBitmap, color);
      }
    }
  }

  espert->oled.update();
  ESP.wdtFeed();
}

void Menu::resetArrow() {
  arrowTime = 0.0f;
  arrowOffset = 0.0f;
  resetGameTime();
}

void Menu::update() {
  if (game) {
    if (game->isBackToMenuEnabled()) {
      if (!backToMenu) {
        backToMenu = true;
        resetArrow();
      }
    } else if (backToMenu) {
      backToMenu = false;
    }

    game->update();

    if (game->isExit()) {
      delete(game);
      game = NULL;
      backToMenu = false;
      readVolume();
      resetArrow();
    }
  } else {
    if (isButtonPressed[BUTTON_LEFT]) {
      pressButtonTime += elapsedTime;
      moveRight();
    } else if (isButtonPressed[BUTTON_RIGHT]) {
      pressButtonTime += elapsedTime;
      moveLeft();
    } else {
      pressButtonTime = 0.0f;
    }

    if (targetOffset != 0.0f) {
      if (pressButtonTime < 500.0f) {
        offset = lerp(0.4f, offset, targetOffset);
      } else {
        pressButtonTime = 500.0f;
        offset += targetOffset * (elapsedTime / 300.0f);

        if ((targetOffset < 0.0f && offset <= targetOffset) || (targetOffset > 0.0f && offset >= targetOffset)) {
          offset = 0.0f;
          currentGame = targetGame;

          if (targetOffset < 0.0f) {
            targetOffset = 0.0f;
            moveLeft();
          } else {
            targetOffset = 0.0f;
            moveRight();
          }
        }
      }

      if (pressButtonTime == 0.0f && offset == targetOffset) {
        offset = 0.0f;
        targetOffset = 0.0f;
        currentGame = targetGame;
      }
    }

    // sound
    if (isSoundEnabled && isGamepadEnabled && (pressedButton == BUTTON_UP || pressedButton == BUTTON_DOWN)) {
      if (pressedButton == BUTTON_UP) {
        increaseVolume();
      } else if (pressedButton == BUTTON_DOWN) {
        decreaseVolume();
      }
    }
  }

  if (!game || (game && backToMenu)) {
    // arrow
    arrowTime += 1.5f * (elapsedTime / 1000.0f);

    if (arrowTime >= 1.0f) {
      arrowTime -= 1.0f;
    }

    arrowOffset = (sin(M_PI * 2.0f * (arrowTime - 0.25f)) + 1.0f) * 0.5f; // 0.0 to 1.0
    arrowOffset *= 10.0f; // 10 pixels

    updateGameTime(game ? false : true);
  }
}
