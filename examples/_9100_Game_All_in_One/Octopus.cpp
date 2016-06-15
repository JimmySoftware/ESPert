// Game & Watch Octopus Simulator for ESPresso Lite

#include "Octopus.h"

using namespace octopus;

Octopus::Octopus() {
  memset(diverStepImage, NULL, sizeof(diverStepImage));
  memset(&isDiverStepVisibled, false, sizeof(isDiverStepVisibled));
  memset(&isRemainderVisibled, true, sizeof(isRemainderVisibled));
  memset(hourDigitImage, NULL, sizeof(hourDigitImage));
  memset(&isHourDigitVisibled, false, sizeof(isHourDigitVisibled));
  memset(minuteDigitImage, NULL, sizeof(minuteDigitImage));
  memset(&isMinuteDigitVisibled, false, sizeof(isMinuteDigitVisibled));

  isMissDiverVisibled = false;
  missArmImage = NULL;
  isMissArmVisibled = false;
  missLegsImage = NULL;
  isMissLegsVisibled = false;

  gameTypeImage = NULL;
  isGameTypeVisibled = false;

  gameAHighScore = 0;
  gameBHighScore = 0;
  gameSpeed = 50;
  gameLoopTime = 0l;
  gameType = GAME_NONE;
  isBagVisibled = false;
  miss = 0;
  isPlaying = false;
  addBonusScore = 0;
  secondCount = 0;
  isScoreVisibled = false;
  onTheBoatCount = 10;
  caught = 0;
  clearMissCount = 0;
  diverPosition = 0;
  isJustStarted = true;
  isShowResetScreen = true;
  showHighScoreTime = 0;
  isColonVisibled = false;
  isAMVisibled = false;
  isPMVisibled = false;

  isRandomTentacle = false;
  currentTentacle = 0;
  memset(&isEndOfTentacle, false, sizeof(isEndOfTentacle));
  memset(&tentacleDirection, 0, sizeof(tentacleDirection));
  memset(&tentacleCount, 0, sizeof(tentacleCount));
  memset(&isTentacleVisibled, false, sizeof(isTentacleVisibled));
  tentacle = 0;
  diverPositionIndex = 0;
  isNextTentacle = false;
}

void Octopus::addScore() {
  if (isPlaying && caught == 0) {
    if (score == maxScore[gameIndex]) {
      checkHighScore();
      score = 1;
    } else {
      score++;
    }

    showScore(score);

    if (score == 200 || score == 500) {
      if (miss == 1) {
        isRemainderVisibled[0] = true;
      } else if (miss == 2) {
        isRemainderVisibled[0] = true;
        isRemainderVisibled[1] = true;
      }

      playSound(3);
      clearMissCount = 16;
    }

    checkSpeed();
  }
}

void Octopus::autoMove() {
  if (!isBagVisibled || (isBagVisibled && diverPosition == 8) || (isBagVisibled && diverPosition == 0 && addBonusScore == 1)) {
    if (isBagVisibled && diverPosition == 0 && addBonusScore == 1) {
      addBonusScore = 0;
      isBagVisibled = false;
    }

    moveRight();
  } else {
    if (addBonusScore > 1) {
      checkBonusScore();
    } else {
      moveLeft();
    }
  }

  for (int i = 0; i < 4; i++) {
    checkTentacle();
  }
}

void Octopus::autoPlayScreen() {
  if (isSecondChanged()) {
    showTime();

    if (caught > 0) {
      checkCaught();
    } else {
      autoMove();
    }
  }
}

void Octopus::pressButton() {
  if (syncInternetTimeStep == SYNC_DELAY || syncInternetTimeStep == SYNC_FINISHED) {
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
            if (pressedButton == BUTTON_NONE && (addBonusScore == 0 || !isPlaying)) {
              pressedButton = i;

              if (syncInternetTimeStep == SYNC_FINISHED) {
                if (!isPlaying || (isPlaying && (miss == 4 || (caught == 0 && (i == BUTTON_LEFT || (i == BUTTON_RIGHT && ((clearMissCount > 0 && diverPosition > 0 && diverPosition <= 5) || (clearMissCount == 0 && diverPosition <= 5)))))))) {
                  if (!isPlaying || miss == 4) {
                    showHighScoreTime = 1;
                    if (((isGamepadEnabled && (i == BUTTON_A || i == BUTTON_B)) || (!isGamepadEnabled && (i == BUTTON_LEFT || i == BUTTON_RIGHT))) && (!isPlaying || (isPlaying && miss == 4))) {
                      showHighScore((pressedButton == BUTTON_A || pressedButton == BUTTON_LEFT) ? GAME_A : GAME_B);
                    }
                  } else if (i == BUTTON_LEFT) {
                    moveLeft();
                  } else if (i == BUTTON_RIGHT) {
                    moveRight();
                  }
                }
              }
            } else if (!isPlaying) {
              if (isGameTypeVisibled) {
                showHighScoreTime = 0;
                isGameTypeVisibled = false;
              }

              if (!isGamepadEnabled && (i == BUTTON_LEFT || i == BUTTON_RIGHT)) {
                toggleVolume();
              }
            }
          } else {
            if (isMenuEnabled && isGamepadEnabled && !isPlaying && !isGameTypeVisibled && pressedButton == BUTTON_LEFT && i == BUTTON_LEFT) {
              isRequestingExit = true;
            } else if (isSoundEnabled && isVolumeChanged > 0.0f) {
              showHighScoreTime = 0;
              isGameTypeVisibled = false;

              if ((!isGamepadEnabled && (i == BUTTON_LEFT || i == BUTTON_RIGHT)) || (isGamepadEnabled && (i == BUTTON_UP || i == BUTTON_DOWN))) {
                playSound(4);
              }
            } else if (syncInternetTimeStep == SYNC_FINISHED && showHighScoreTime > 0) {
              showHighScoreTime = 0;
              isGameTypeVisibled = false;

              if (((isGamepadEnabled && (i == BUTTON_A || i == BUTTON_B)) || (!isGamepadEnabled && (i == BUTTON_LEFT || i == BUTTON_RIGHT))) && pressedButton == i) {
                resetGame();
                gameType = (i == BUTTON_A || i == BUTTON_LEFT) ? GAME_A : GAME_B;
                startGame();
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

void Octopus::checkBonusScore() {
  if (addBonusScore > 0) {
    onTheBoatCount = 10;

    if (--addBonusScore > 0) {
      addScore();

      if (clearMissCount == 0) {
        playSound(1);
      }
    } else {
      isBagVisibled = false;
      checkSpeed();
    }

    if (addBonusScore == 2) {
      diverStepImage[diverPosition] = onTheBoatBitmap[1];
    } else if (addBonusScore == 3 || addBonusScore <= 1) {
      diverStepImage[diverPosition] = onTheBoatBitmap[0];
    }
  }
}

void Octopus::checkCaught() {
  if (caught > 0) {
    if (--caught == 10) {
      playSound(2);
      checkSpeed();
    } else if (caught == 8) {
      miss++;
      checkDiver();
      isDiverStepVisibled[(diverPosition < 5) ? diverPosition : 5] = false;
      isMissDiverVisibled = true;
      missArmImage = missArmBitmap[0];
      missLegsImage = missLegsBitmap[0];
      isMissArmVisibled = true;
      isMissLegsVisibled = true;
      isTentacleVisibled[2][0] = true;
      isTentacleVisibled[2][1] = true;
      isTentacleVisibled[2][2] = false;
      isTentacleVisibled[2][3] = false;
      isTentacleVisibled[2][4] = false;
      tentacleCount[2] = 2;
      isEndOfTentacle[2] = false;

      if (tentacleCount[3] < 2) {
        isTentacleVisibled[3][0] = true;
        isTentacleVisibled[3][1] = true;
        tentacleCount[3] = 2;
        isEndOfTentacle[3] = false;
      }
    } else if (caught > 0 && caught < 8 && (caught % 2) == 0) {
      missArmImage = missArmBitmap[0];
      missLegsImage = missLegsBitmap[0];

      if (miss <= 3) {
        playSound(1);
      }

      if (caught == 4 && miss == 1) {
        isRemainderVisibled[0] = false;
        isRemainderVisibled[1] = true;
      }
    } else if (caught > 0 && caught < 8 && (caught % 2) == 1) {
      missArmImage = missArmBitmap[1];
      missLegsImage = missLegsBitmap[1];

      if (miss == 4) {
        caught = 7;
      } else {
        playSound(1);
      }
    } else if (caught == 0) {
      if (miss < 3) {
        isMissDiverVisibled = false;
        isMissArmVisibled = false;
        isMissLegsVisibled = false;
        isBagVisibled = false;
        onTheBoatCount = 10;
        secondCount = 0;
      }

      checkMiss();
      checkSpeed();
    }
  }
}

void Octopus::checkClearMiss() {
  if (clearMissCount > 0) {
    clearMissCount--;

    if (miss == 1) {
      isRemainderVisibled[0] = !isRemainderVisibled[0];
    } else if (miss == 2) {
      isRemainderVisibled[0] = !isRemainderVisibled[0];
      isRemainderVisibled[1] = !isRemainderVisibled[1];
    }

    if (isRemainderVisibled[0]) {
      playSound(3);
    }

    if (clearMissCount == 0) {
      miss = 0;
    }
  }
}

void Octopus::checkDiver() {
  if (miss == 1 || miss == 2) {
    isRemainderVisibled[1] = false;
    diverStepImage[0] = onTheBoatBitmap[0];
    isDiverStepVisibled[0] = true;
  }
}

void Octopus::checkHighScore() {
  if (gameType == GAME_A && score > gameAHighScore) {
    gameAHighScore = score;
    highScore = gameAHighScore;
    writeHighScore();
  } else if (gameType == GAME_B && score > gameBHighScore) {
    gameBHighScore = score;
    highScore = gameBHighScore;
    writeHighScore(String(maxScore[gameIndex]).length());
  }
}

void Octopus::checkMiss() {
  if (miss <= 2) {
    diverStepImage[0] = onTheBoatBitmap[0];
    isDiverStepVisibled[0] = true;
  }

  switch (miss) {
    case 0:
      isRemainderVisibled[0] = true;
      isRemainderVisibled[1] = true;
      break;

    case 1:
      isRemainderVisibled[0] = false;
      isRemainderVisibled[1] = true;
      tryAgain();
      break;

    case 2:
      isRemainderVisibled[0] = false;
      isRemainderVisibled[1] = false;
      tryAgain();
      break;

    default:
      checkHighScore();

      if (isPlaying) {
        caught = 7;
        miss = 4;
        secondCount = 0;
      } else {
        if (caught == 0) {
          isMissDiverVisibled = false;
          isMissArmVisibled = false;
          isMissLegsVisibled = false;
          isBagVisibled = false;
          onTheBoatCount = 10;
          secondCount = 0;
          isRemainderVisibled[0] = true;
          isRemainderVisibled[1] = true;
          diverStepImage[0] = onTheBoatBitmap[0];
          isDiverStepVisibled[0] = true;
        }

        addBonusScore = 0;
        miss = 0;
        tryAgain();
      }
      break;
  }
}

bool Octopus::checkShowHighScoreTime() {
  bool finished = true;

  if (showHighScoreTime > 0 && ((isGamepadEnabled && (pressedButton == BUTTON_A || pressedButton == BUTTON_B)) || (!isGamepadEnabled && (pressedButton == BUTTON_LEFT || pressedButton == BUTTON_RIGHT)))) {
    if (++showHighScoreTime > 3) {
      showHighScoreTime = 0;
      isGameTypeVisibled = false;
    } else {
      showHighScore((pressedButton == BUTTON_A || pressedButton == BUTTON_LEFT) ? GAME_A : GAME_B);
      finished = false;
    }
  }

  return finished;
}

void Octopus::checkSpeed() {
  if (caught > 0 || addBonusScore > 0 || clearMissCount > 0) {
    gameSpeed = diverSpeed;
  } else {
    gameSpeed = 50;

    switch (gameType) {
      case GAME_A:
        gameSpeed = gameASpeed;

        if (gameSpeed > 30 && ((score >= 70 && score < 100) || (score >= 160 && score < 190) || (score >= 250 && score < 280) || (score >= 330 && score < 360) || (score >= 430 && score < 460) || (score >= 520 && score < 560) || (score >= 610 && score < 650) || (score >= 710 && score < 740) || (score >= 810 && score < 830) || (score >= 930 && score < 970))) {
          gameSpeed -= 30;
        } else if (gameSpeed > 60 && ((score >= 190 && score < 200) || (score >= 280 && score < 300) || (score >= 560 && score < 600) || (score >= 650 && score < 690) || (score >= 740 && score < 780))) {
          gameSpeed -= 60;
        } else if (gameSpeed > 50 && ((score >= 360 && score < 390) || (score >= 460 && score < 490) || (score >= 830 && score < 870))) {
          gameSpeed -= 50;
        } else if (gameSpeed > 80 && ((score >= 390 && score < 400) || (score >= 490 && score < 500) || (score >= 690 && score < 700) || (score >= 780 && score < 800) || (score >= 870 && score < 900) || (score >= 970 && score <= maxScore[gameIndex]))) {
          gameSpeed -= 80;
        }
        break;

      case GAME_B:
        gameSpeed = gameBSpeed;

        if (gameSpeed > 30 && ((score >= 30 && score < 70) || (score >= 130 && score < 160) || (score >= 220 && score < 250) || (score >= 330 && score < 360) || (score >= 430 && score < 460) || (score >= 520 && score < 560) || (score >= 610 && score < 650) || (score >= 710 && score < 740) || (score >= 830 && score < 870) || (score >= 930 && score < 970))) {
          gameSpeed -= 30;
        } else if (gameSpeed > 60 && ((score >= 70 && score < 100) || (score >= 160 && score < 190) || (score >= 250 && score < 280) || (score >= 360 && score < 390) || (score >= 460 && score < 490) || (score >= 560 && score < 600) || (score >= 650 && score < 690) || (score >= 740 && score < 780) || (score >= 870 && score < 900))) {
          gameSpeed -= 60;
        } else if (gameSpeed > 80 && ((score >= 190 && score < 200) || (score >= 280 && score < 300) || (score >= 390 && score < 400) || (score >= 490 && score < 500) || (score >= 690 && score < 700) || (score >= 780 && score < 800) || (score >= 970 && score < maxScore[gameIndex]))) {
          gameSpeed -= 80;
        }
        break;
    }

    if (gameSpeed < 0) {
      gameSpeed = 0;
    }
  }
}

void Octopus::checkTentacle() {
  bool isPlaySound = false;
  tentacle = 0;
  diverPositionIndex = 0;
  isNextTentacle = false;

  if ((currentTentacle > 1 && ((tentacleCount[currentTentacle] == 0 && random(5) <= 2) || tentacleCount[currentTentacle] > 0)) || (currentTentacle <= 1 && ((tentacleCount[0] == 0 && tentacleCount[1] == 0 && random(5) <= 2) || (tentacleCount[currentTentacle] > 0)))) {
    isNextTentacle = false;

    if (currentTentacle == 0 || currentTentacle == 1) {
      if (tentacleCount[0] == 0 && tentacleCount[1] == 0 && isRandomTentacle) {
        currentTentacle = random(2);
        isRandomTentacle = false;;
      } else if (tentacleCount[0] == 0 && tentacleCount[1] == 0 && !isRandomTentacle) {
        isNextTentacle = true;
        isRandomTentacle = true;
      }
    }

    tentacle = currentTentacle;

    if (isNextTentacle) {
      isPlaySound = (addBonusScore == 0) ? true : false;
    } else if (!isNextTentacle && tentacleDirection[tentacle] == 0) {
      isTentacleVisibled[tentacle][tentacleCount[tentacle]] = true;
      isPlaySound = (addBonusScore == 0) ? true : false;

      if (++tentacleCount[tentacle] == maxTentacleCount[tentacle]) {
        isEndOfTentacle[tentacle] = true;
        diverPositionIndex = (diverPosition > 5) ? 5 : diverPosition;

        if (diverPositionIndex == tentacle + 1) {
          diverStepImage[diverPositionIndex] = diverBitmap[((diverPosition - 1) * 2) + isBagVisibled];
          isPlaySound = false;
          gotCaught();
        }

        tentacleDirection[tentacle] = 1;
      } else {
        isEndOfTentacle[tentacle] = false;
      }
    } else if (!isNextTentacle) {
      if (gameType == GAME_B && tentacleDirection[tentacle] == 1 && tentacleCount[tentacle] == 1 && random(10) == 0) {
        tentacleDirection[tentacle] = 0;
        isPlaySound = (addBonusScore == 0) ? true : false;
      } else {
        tentacleCount[tentacle]--;
        isTentacleVisibled[tentacle][tentacleCount[tentacle]] = false;
        isEndOfTentacle[tentacle] = false;
        isPlaySound = (addBonusScore == 0) ? true : false;

        if (tentacle < 2) {
          isRandomTentacle = false;
        }

        if (tentacleCount[tentacle] == 0) {
          tentacleDirection[tentacle] = 0;
        }
      }
    }
  } else {
    tentacle = currentTentacle;
    isPlaySound = (addBonusScore == 0) ? true : false;
  }

  if (isPlaySound) {
    playSound(0);
  }

  currentTentacle = tentacle + 1;

  if (currentTentacle > 4) {
    currentTentacle = 0;
  }
}

String floatToString(float value, int length, int decimalPalces) {
  String stringValue = String(value, decimalPalces);
  String prefix = "";

  for (int i = 0; i < length - stringValue.length(); i++) {
    prefix += " ";
  }

  return prefix + stringValue;
}

void Octopus::gotCaught() {
  if (clearMissCount == 0) {
    caught = isPlaying ? 11 : 9;
  }
}

void Octopus::initGame() {
  gameIndex = GAME_OCTOPUS;
  resetGame();
  gameAHighScore = readHighScore();
  gameBHighScore = readHighScore(String(maxScore[gameIndex]).length());
  readVolume();

  if (isMenuEnabled) {
    syncInternetTimeStep = SYNC_FINISHED;
    showResetScreenTime = 0.0f;
  } else {
    showResetScreen();
    showResetScreenTime = 3000.0f;
    syncInternetTimeStep = SYNC_SHOW_RESET_SCREEN;
  }
}

String intToString(int value, int length, String prefixChar) {
  String stringValue = String(value);
  String prefix = "";

  for (int i = 0; i < length - stringValue.length(); i++) {
    prefix += prefixChar;
  }

  return prefix + stringValue;
}

bool Octopus::isBackToMenuEnabled() {
  return (isMenuEnabled && (syncInternetTimeStep == SYNC_DELAY || syncInternetTimeStep == SYNC_FINISHED) && !isPlaying && !isGameTypeVisibled) ? true : false;
}

void Octopus::moveLeft() {
  if (diverPosition >= 1) {
    if (diverPosition > 5) {
      diverPosition = 5;
    }

    if (diverPosition > 1) {
      isDiverStepVisibled[diverPosition] = false;
      diverPosition--;
      diverStepImage[diverPosition] = diverBitmap[((diverPosition - 1) * 2) + isBagVisibled];
      isDiverStepVisibled[diverPosition] = true;

      if (isEndOfTentacle[diverPosition - 1]) {
        gotCaught();
      }
    } else if (isBagVisibled && clearMissCount == 0) {
      isDiverStepVisibled[diverPosition] = false;
      diverPosition--;
      addBonusScore = 4;
      diverStepImage[diverPosition] = onTheBoatBitmap[1];
      isDiverStepVisibled[diverPosition] = true;
    }
  }
}

void Octopus::moveRight() {
  onTheBoatCount = 0;

  if (diverPosition < 5) {
    isDiverStepVisibled[diverPosition] = false;
    diverPosition++;
    diverStepImage[diverPosition] = diverBitmap[((diverPosition - 1) * 2) + isBagVisibled];
    isDiverStepVisibled[diverPosition] = true;

    if (isEndOfTentacle[diverPosition - 1]) {
      gotCaught();
    }
  } else if (diverPosition < 10 && clearMissCount == 0) {
    if (++diverPosition == 9) {
      diverPosition = 5;
    } else if (diverPosition == 8) {
      isBagVisibled = true;

      if (isPlaying && caught == 0) {
        playSound(1);
        addScore();
      }
    }

    diverStepImage[5] = diverBitmap[((diverPosition - 1) * 2) + isBagVisibled];

    if (isEndOfTentacle[5 - 1]) {
      gotCaught();
    }
  }
}

void Octopus::playSound(int index) {
  if (isSoundEnabled && (isPlaying || index == 4)) {
    int frequency = 25;

    switch (index) {
      case 0:
        soundDuration = 11.0f;
        break;

      case 1:
        soundDuration = 97.0f;
        break;

      case 2:
        soundDuration = 428.0f;
        break;

      case 3:
        soundDuration = 191.0f;
        break;

      case 4:
        soundDuration = 100.0f;
        break;

      default:
        soundDuration = 0.0f;
        frequency = 0;
        break;
    }

    espert->buzzer.on((int)constrain(frequency * volume, 0.0f, 25.0f));
  }
}

void Octopus::render() {
  if (!isMenuEnabled) {
    espert->oled.clear(false);
  }

  espert->oled.setColor(ESPERT_WHITE);
  espert->oled.drawBitmap(0, 0, 128, 64, backgroundBitmap, false);

  if (isGameTypeVisibled) {
    espert->oled.drawBitmap(0, 56, 32, 8, gameTypeImage, false);
  }

  if (isShowResetScreen) {
    frameRate = 0;
  }

  renderFPS(117, 56, 8, 8, -2, (const uint8_t*)numberBitmap, NULL, ESPERT_WHITE, 103, 58);

  espert->oled.setColor(ESPERT_BLACK);

  if (!isPlaying) {
    renderVolume(1, 1, ESPERT_BLACK);
    renderBattery(screenSize.width - batterySize.width - 1, 1, ESPERT_BLACK);
  }

  for (int i = 0; i < 6; i++) {
    if (isDiverStepVisibled[i]) {
      espert->oled.drawBitmap(diverStepPosition[i].x, diverStepPosition[i].y, 16, 16, diverStepImage[i], false);
    }

    for (int j = 0; j < 5; j++) {
      if (i < maxTentacleCount[j] && isTentacleVisibled[j][i]) {
        espert->oled.drawBitmap(tentaclePosition[j][i].x, tentaclePosition[j][i].y, 8, 8, tentacleBitmap[j][i], false);
      }
    }

    if (i < 2) {
      if (isRemainderVisibled[i]) {
        espert->oled.drawBitmap(remainderPosition[i].x, remainderPosition[i].y, 16, 16, remainderBitmap, false);
      }

      if (isHourDigitVisibled[i]) {
        espert->oled.drawBitmap(hourDigitPosition[i].x, hourDigitPosition[i].y, 8, 8, hourDigitImage[i], false);
      }

      if (isMinuteDigitVisibled[i]) {
        espert->oled.drawBitmap(minuteDigitPosition[i].x, minuteDigitPosition[i].y, 8, 8, minuteDigitImage[i], false);
      }

      if (i == 0) {
        if (isColonVisibled) {
          espert->oled.drawBitmap(74, 1, 8, 8, colonBitmap, false);
        }

        if (isAMVisibled) {
          espert->oled.drawBitmap(52, 1, 16, 8, amBitmap, false);
        }

        if (isPMVisibled) {
          espert->oled.drawBitmap(52, 5, 16, 8, pmBitmap, false);
        }

        if (isMissDiverVisibled) {
          espert->oled.drawBitmap(missDiverPosition.x, missDiverPosition.y, 16, 16, missDiverBitmap, false);
        }

        if (isMissArmVisibled) {
          espert->oled.drawBitmap(missArmPosition.x, missArmPosition.y, 8, 8, missArmImage, false);
        }

        if (isMissLegsVisibled) {
          espert->oled.drawBitmap(missLegsPosition.x, missLegsPosition.y, 16, 16, missLegsImage, false);
        }

        if (isShowResetScreen) {
          espert->oled.drawBitmap(diverStepPosition[0].x, diverStepPosition[0].y, 16, 16, onTheBoatBitmap[0], false);
          espert->oled.drawBitmap(diverStepPosition[5].x, diverStepPosition[5].y, 16, 16, diverBitmap[11], false);
          espert->oled.drawBitmap(diverStepPosition[5].x, diverStepPosition[5].y, 16, 16, diverBitmap[15], false);
          espert->oled.drawBitmap(missArmPosition.x, missArmPosition.y, 8, 8, missArmBitmap[0], false);
          espert->oled.drawBitmap(missLegsPosition.x, missLegsPosition.y, 16, 16, missLegsBitmap[0], false);
        }
      }
    }
  }

  if (!isMenuEnabled) {
    espert->oled.update();
  }

  if (syncInternetTimeStep == SYNC_SHOW_RESET_SCREEN) {
    isSyncInternetTime = true;
    syncInternetTimeStep = (isTimeVisibled ? SYNC_READ_INTERNET_TIME : SYNC_DELAY);
  }
}

void Octopus::resetGame() {
  isShowResetScreen = false;
  gameSpeed = 50;
  gameLoopTime = 0l;
  gameType = GAME_NONE;
  isBagVisibled = false;
  score = 0;
  miss = 0;
  isPlaying = false;
  addBonusScore = 0;
  secondCount = 0;
  isScoreVisibled = false;
  onTheBoatCount = 10;
  caught = 0;
  clearMissCount = 0;
  diverPosition = 0;
  isRandomTentacle = false;
  currentTentacle = 0;
  isMissDiverVisibled = false;
  isMissArmVisibled = false;
  isMissLegsVisibled = false;
  isGameTypeVisibled = false;
  isColonVisibled = false;
  isAMVisibled = false;
  isPMVisibled = false;
  memset(&isDiverStepVisibled, false, sizeof(isDiverStepVisibled));
  memset(&isRemainderVisibled, true, sizeof(isRemainderVisibled));
  memset(&isHourDigitVisibled, false, sizeof(isHourDigitVisibled));
  memset(&isMinuteDigitVisibled, false, sizeof(isMinuteDigitVisibled));
  memset(&isEndOfTentacle, false, sizeof(isEndOfTentacle));
  memset(&tentacleDirection, 0, sizeof(tentacleDirection));
  memset(&tentacleCount, 0, sizeof(tentacleCount));
  memset(&isTentacleVisibled, false, sizeof(isTentacleVisibled));
  diverStepImage[0] = onTheBoatBitmap[0];
  isDiverStepVisibled[0] = true;
  showTime();
}

void Octopus::showGameType(byte gameTypeValue) {
  isGameTypeVisibled = false;

  if (gameTypeValue == GAME_A || gameTypeValue == GAME_B) {
    isGameTypeVisibled = true;
    gameTypeImage = gameTypeBitmap[gameTypeValue - 1];
  }
}

void Octopus::showHighScore(byte gameTypeValue) {
  isGameTypeVisibled = true;
  gameTypeImage = gameTypeBitmap[gameTypeValue - 1];
  showScore((gameTypeValue == GAME_A) ? gameAHighScore : gameBHighScore);
}

void Octopus::showResetScreen(byte gameTypeValue) {
  isShowResetScreen = true;

  if (gameTypeValue == GAME_A || gameTypeValue == GAME_B) {
    isGameTypeVisibled = true;
    gameTypeImage = gameTypeBitmap[gameTypeValue - 1];
    showScore((gameTypeValue == 1) ? gameAHighScore : gameBHighScore);
  } else {
    isGameTypeVisibled = false;
    isAMVisibled = true;
    isPMVisibled = false;
    isColonVisibled = true;
    memset(&isHourDigitVisibled, true, sizeof(isHourDigitVisibled));
    memset(&isMinuteDigitVisibled, true, sizeof(isMinuteDigitVisibled));

    for (int i = 0; i < 2; i++) {
      hourDigitImage[i] = numberBitmap[i + 1];
      minuteDigitImage[i] = numberBitmap[0];
    }
  }

  memset(&isDiverStepVisibled, true, sizeof(isDiverStepVisibled));
  memset(&isRemainderVisibled, true, sizeof(isRemainderVisibled));
  memset(&isTentacleVisibled, true, sizeof(isTentacleVisibled));
  isMissDiverVisibled = true;
  isMissArmVisibled = true;
  isMissLegsVisibled = true;
  missArmImage = missArmBitmap[1];
  missLegsImage = missLegsBitmap[1];

  for (int i = 0; i < 6; i++) {
    diverStepImage[i] = (i == 0) ? onTheBoatBitmap[1] : diverBitmap[((i - 1) * 2) + 1];
  }
}

void Octopus::showScore(short value) {
  String scoreString = intToString(value, 3, "0");
  isScoreVisibled = true;
  isAMVisibled = false;
  isPMVisibled = false;
  isColonVisibled = false;
  isHourDigitVisibled[0] = false;

  if (scoreString.charAt(0) != '0') {
    hourDigitImage[1] = numberBitmap[scoreString.charAt(0) - '0'];
    isHourDigitVisibled[1] = true;
    minuteDigitImage[0] = numberBitmap[scoreString.charAt(1) - '0'];
    isMinuteDigitVisibled[0] = true;
  } else {
    isHourDigitVisibled[1] = false;

    if (scoreString.charAt(1) != '0') {
      minuteDigitImage[0] = numberBitmap[scoreString.charAt(1) - '0'];
      isMinuteDigitVisibled[0] = true;
    } else {
      isMinuteDigitVisibled[0] = false;
    }
  }

  minuteDigitImage[1] = numberBitmap[scoreString.charAt(2) - '0'];
  isMinuteDigitVisibled[1] = true;
}

void Octopus::showTime() {
  if (checkShowHighScoreTime()) {
    showHighScoreTime = 0;

    if (isTimeVisibled) {
      isColonVisibled = (seconds % 2 == 0) ? true : false;
      isAMVisibled = (hours < 12) ? true : false;
      isPMVisibled = !isAMVisibled;
      int h = hours % 12;
      String hh = intToString((h == 0) ? 12 : h, 2, "0");
      String mm = intToString(minutes, 2, "0");
      memset(&isHourDigitVisibled, true, sizeof(isHourDigitVisibled));
      memset(&isMinuteDigitVisibled, true, sizeof(isMinuteDigitVisibled));

      for (int i = 0; i < 2; i++) {
        hourDigitImage[i] = numberBitmap[hh.charAt(i) - '0'];
        minuteDigitImage[i] = numberBitmap[mm.charAt(i) - '0'];
      }

      if (hh.charAt(0) == '0') {
        isHourDigitVisibled[0] = false;
      }
    }
  }
}

void Octopus::startGame() {
  if (!isPlaying) {
    gameLoopTime = 0l;
    showGameType(gameType);
    isMissDiverVisibled = false;
    isMissArmVisibled = false;
    isMissLegsVisibled = false;
    isPlaying = true;
    isBagVisibled = false;
    clearMissCount = 0;
    isRandomTentacle = false;
    memset(&isEndOfTentacle, false, sizeof(isEndOfTentacle));
    memset(&tentacleDirection, 0, sizeof(tentacleDirection));

    for (int i = 0; i < 5; i++) {
      isDiverStepVisibled[i + 1] = false;

      if (tentacleCount[i] > 0) {
        for (int j = tentacleCount[i] - 1; j >= 0; j--) {
          isTentacleVisibled[i][j] = false;
        }
      }

      tentacleCount[i] = 0;
    }

    currentTentacle = 0;
    addBonusScore = 0;
    diverPosition = 0;
    onTheBoatCount = 10;
    secondCount = 0;
    score = 0;
    caught = 0;
    showScore(score);
    checkSpeed();
    miss = 0;
    checkDiver();
    checkMiss();
  }
}

void Octopus::tryAgain() {
  for (int i = 1; i < 6; i++) {
    isDiverStepVisibled[i] = false;
  }

  diverPosition = 0;
}

void Octopus::update() {
  if (syncInternetTimeStep == SYNC_READ_INTERNET_TIME) {
    syncInternetTimeStep = SYNC_DELAY;
    if (readInternetTime()) {
      showResetScreenTime = 0.0f;
    }
  } else {
    if (!isPlaying) {
      if (isSoundEnabled && (isGamepadEnabled && (pressedButton == BUTTON_UP || pressedButton == BUTTON_DOWN))) {
        if (pressedButton == BUTTON_UP) {
          increaseVolume();
        } else if (pressedButton == BUTTON_DOWN) {
          decreaseVolume();
        }
      }
    }

    if (syncInternetTimeStep == SYNC_DELAY) {
      updateGameTime();
      showResetScreenTime -= elapsedTime;

      if (showResetScreenTime <= 0.0f) {
        showResetScreenTime = 0.0f;
        resetGame();
        gameType = GAME_NONE;
        resetGameTime();
        timeSyncLastFrameTime = lastFrameTime;
        syncInternetTimeStep = SYNC_FINISHED;
      }
    } else if (syncInternetTimeStep == SYNC_FINISHED) {
      updateGameTime();

      // game loop
      if (!isShowResetScreen) {
        gameLoopTime += elapsedTime;

        if (gameLoopTime >= gameSpeed) {
          gameLoopTime -= gameSpeed;

          if (!isPlaying) {
            autoPlayScreen();
          } else {
            if (isSecondChanged()) {
              if (miss == 4 && ++secondCount > 3) {
                showHighScoreTime = 0;
                resetGame();
                tryAgain();
              }

              if (caught == 0 && clearMissCount == 0 && onTheBoatCount > 0) {
                secondCount++;

                if (--onTheBoatCount == 0) {
                  moveRight();
                  secondCount = 0;
                }
              }
            }

            if (caught == 0) {
              checkBonusScore();

              if (clearMissCount == 0) {
                checkTentacle();

                if (diverPosition > 5) {
                  moveRight();
                }
              } else {
                checkClearMiss();
              }
            } else if (caught > 0) {
              checkCaught();
            }
          }
        }
      }
    }
  }
}
