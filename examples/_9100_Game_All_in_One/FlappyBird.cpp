// Flappy Bird Simulator for ESPresso Lite

#include "FlappyBird.h"

using namespace flappyBird;

FlappyBird::FlappyBird() {
  // game
  gameMode = GAME_MODE_TITLE;
  gameOverVelocity = 0.0f;
  gameOverPosition = {0.0f};
  isGameOver = false;
  isCheckHighScore = false;
  titleTime = 0.0f;
  getReadyTime = 0.0f;
  gameOverTime = 0.0f;
  isAutoPlay = false;

  // bird
  birdSize = {16, 16};
  birdAnimationSpeed = 8.0f / 1000.0f;
  birdFrame = 0.0f;
  birdDegrees = 0.0f;
  birdFirstFrame = 1; // change this value to 0 to enable birdFrame 0
  birdFrameDirection = 1;
  isDropping = false;
  heightRatio = 0.0f;
  gravity = 9.8f;
  velocity = 0.0f;
  initialVelocity = -2.5f;
  birdPosition = {21, (screenSize.height - birdSize.height) * 0.5f};
  dropPosition = 0.0f;
  isHit = false;
  birdCollisionOffset = {2.0f, 2.0f};

  // land
  landSize = {128, 4};
  landSpeed = 25.0f / 1000.0f;
  landOffset = {0.0f, screenSize.height - landSize.height};
  landPosition = screenSize.height - birdSize.height - landSize.height;

  // score
  isNewHighScore = false;
  scorePosition[0] = {0, 1};
  scorePosition[1] = {0, 1};
  scorePosition[2] = {0, 1};
  memset(scoreDigitImage, NULL, sizeof(scoreDigitImage));
  memset(scoreDigitMaskImage, NULL, sizeof(scoreDigitMaskImage));

  // score panel
  maxScorePanelOffset = 64.0f;
  scorePanelOffset = 0.0f;
  scorePanelScore = 0.0f;
  scorePanelHighScore = 0.0f;
  memset(scorePanelHighScoreDigitImage, NULL, sizeof(scorePanelHighScoreDigitImage));
  memset(scorePanelScoreDigitImage, NULL, sizeof(scorePanelScoreDigitImage));
  scorePanelMedalImage = NULL;

  // pipe
  pipeSize = {14, 32};
  pipeMinLength = 4;
  heightBetweenPipes = 30;
  pipeRandomFrom = pipeMinLength - 1;
  pipeRandomTo = screenSize.height - landSize.height - pipeMinLength - 1 - heightBetweenPipes;
  memset(pipePosition, 0.0f, sizeof(pipePosition));
  memset(isPipeVisibled, false, sizeof(isPipeVisibled));
  memset(isPipeEnabled, false, sizeof(isPipeEnabled));
  pipeRandomFirstTime = 3000.0f; // milliseconds
  pipeRandomNextTime = 2000.0f; // milliseconds
  pipeRandomTime = 0.0f;
}

void FlappyBird::addScore(int value) {
  score = constrain(score + value, 0, maxScore[gameIndex]);
  setScoreImages();
}

void FlappyBird::changeGameMode(int mode) {
  switch (mode) {
    case GAME_MODE_TITLE:
      readHighScore();
      resetGame();
      titleTime = 0.0f;
      birdPosition.x = (screenSize.width - birdSize.width) * 0.5f;
      birdDegrees = 1;
      break;

    case GAME_MODE_GET_READY:
      getReadyTime = 0.0f;
      resetBird();
      birdDegrees = 1;
      break;

    case GAME_MODE_PLAY:
      break;

    case GAME_MODE_GAME_OVER:
      readHighScore();
      gameOverTime = 0.0f;
      isCheckHighScore = true;
      gameOverVelocity = initialVelocity * 0.7f;
      gameOverPosition.x = 32;
      gameOverPosition.y = 0;
      scorePanelOffset = maxScorePanelOffset;
      scorePanelScore = 0;
      scorePanelHighScore = highScore; // old highscore
      scorePanelMedalImage = NULL;
      setScorePanelImages(scorePanelScoreDigitImage, scorePanelScore);
      setScorePanelImages(scorePanelHighScoreDigitImage, scorePanelHighScore);
      isButtonAllowed = false;
      pressedButton = BUTTON_NONE;
      break;
  }

  gameMode = mode;
}

void FlappyBird::checkHighScore() {
  isNewHighScore = false;
  scorePanelMedalImage = NULL;

  if (score >= 40) {
    scorePanelMedalImage = medalBitmap[MEDAL_PLATINUM];
  } else if (score >= 30) {
    scorePanelMedalImage = medalBitmap[MEDAL_GOLD];
  } else if (score >= 20) {
    scorePanelMedalImage = medalBitmap[MEDAL_SILVER];
  } else if (score >= 10) {
    scorePanelMedalImage = medalBitmap[MEDAL_BRONZE];
  }

  if (score > highScore) {
    isNewHighScore = true;
    highScore = score;

    if (!isAutoPlay) {
      writeHighScore();
    }
  }
}

void FlappyBird::initGame() {
  gameIndex = GAME_FLAPPY_BIRD;
  readHighScore();
  readVolume();
  resetGame();
  changeGameMode(GAME_MODE_TITLE);
}

bool FlappyBird::isBackToMenuEnabled() {
  return (isMenuEnabled && gameMode == GAME_MODE_TITLE) ? true : false;
}

bool FlappyBird::isHitPipe() {
  if (!isHit) {
    float x1 = birdPosition.x + birdCollisionOffset.x;
    float y1 = birdPosition.y + birdCollisionOffset.y;
    float w1 = birdSize.width - (birdCollisionOffset.x * 2.0f);
    float h1 = birdSize.height - (birdCollisionOffset.y * 2.0f);
    float w2 = pipeSize.width;
    float h2 = pipeSize.height;

    for (int i = 0; i < numberOfPipes && !isHit; i++) {
      if (isPipeVisibled[i] && (!isAutoPlay || (isAutoPlay && isPipeEnabled[i]))) {
        float x2 = pipePosition[i].x;

        for (int pipe = 0; pipe < 2 && !isHit; pipe++) {
          float y2 = pipePosition[i].y + (pipe == 0 ? -pipeSize.height : heightBetweenPipes);

          isHit = !(x1 > x2 + w2 || x1 + w1 < x2 || y1 > y2 + h2 || y1 + h1 < y2);

          if (isHit) {
            playSound(SOUND_DIE);
            isButtonAllowed = false;
          }
        }
      }
    }
  }

  return isHit;
}

void FlappyBird::jump() {
  velocity = initialVelocity;
  isDropping = false;
}

void FlappyBird::playSound(int index) {
  if (!isAutoPlay && isSoundInterruptEnabled) {
    isSoundInterruptEnabled = false;
    nextSound = SOUND_NONE;
    nextSoundDelay = 0.0f;
    int frequency = 0;

    switch (index) {
      case SOUND_SCORE:
        frequency = 15;
        soundDuration = 20.0f;
        nextSound = SOUND_SCORE_NEXT;
        nextSoundDelay = 20.0f;
        break;

      case SOUND_SCORE_NEXT:
        frequency = 25;
        soundDuration = 80.0f;
        break;

      case SOUND_DIE:
        frequency = 25;
        soundDuration = 1000.0f;
        break;

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

void FlappyBird::pressButton() {
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

        if (isButtonAllowed || isAutoPlay) {
          if (isPressed) {
            pressedButton = i;

            if (gameMode == GAME_MODE_TITLE && !isGamepadEnabled && i == BUTTON_RIGHT) {
              toggleVolume();
              titleTime = 0.0f;
            } else if (!isAutoPlay && (gameMode == GAME_MODE_GET_READY || gameMode == GAME_MODE_PLAY)) {
              jump();

              if (gameMode == GAME_MODE_GET_READY) {
                changeGameMode(GAME_MODE_PLAY);
              }
            }
          } else {
            if (isVolumeChanged > 0.0f) {
              if ((!isGamepadEnabled && i == BUTTON_RIGHT) || (isGamepadEnabled && (i == BUTTON_UP || i == BUTTON_DOWN))) {
                titleTime = 0.0f;
                playSound(SOUND_VOLUME);
              }
            } else if (i == pressedButton && ((isGamepadEnabled && (i == BUTTON_A || i == BUTTON_B)) || (!isGamepadEnabled && (i == BUTTON_LEFT || i == BUTTON_RIGHT)))) {
              if (isAutoPlay) {
                readHighScore();
                resetGame();
                changeGameMode(GAME_MODE_TITLE);
              } else if (gameMode == GAME_MODE_TITLE) {
                changeGameMode(GAME_MODE_GET_READY);
              } else if (gameMode == GAME_MODE_GAME_OVER) {
                changeGameMode(GAME_MODE_TITLE);
              }
            } else if (isMenuEnabled && isGamepadEnabled && gameMode == GAME_MODE_TITLE && pressedButton == BUTTON_LEFT && i == BUTTON_LEFT) {
              isRequestingExit = true;
            }

            pressedButton = BUTTON_NONE;
          }
        }
        break;
      }
    }
  }
}

void FlappyBird::render() {
  if (!isMenuEnabled) {
    espert->oled.clear(false);
  }

  drawBitmap(0, 0, screenSize.width, screenSize.height, backgroundBitmap, NULL, ESPERT_WHITE);

  switch (gameMode) {
    case GAME_MODE_TITLE:
      drawBitmap(32, 1, 72, 24, titleBitmap, titleMaskBitmap, ESPERT_BLACK);
      renderMakerAsia(40, 51, ESPERT_BLACK);
      renderHighScore();
      drawBitmap(birdPosition.x, birdPosition.y, 16, 16, birdBitmap[(int)birdFrame][(int)birdDegrees], birdMaskBitmap[(int)birdFrame][(int)birdDegrees], ESPERT_BLACK);
      break;

    case GAME_MODE_GET_READY:
      drawBitmap(32, 1, 64, 56, getReadyBitmap, getReadyMaskBitmap, ESPERT_BLACK);
      drawBitmap(birdPosition.x, birdPosition.y, 16, 16, birdBitmap[(int)birdFrame][(int)birdDegrees], birdMaskBitmap[(int)birdFrame][(int)birdDegrees], ESPERT_BLACK);
      break;

    case GAME_MODE_PLAY:
    case GAME_MODE_GAME_OVER:
      for (int i = 0; i < numberOfPipes; i++) {
        if (isPipeVisibled[i]) {
          drawBitmap(pipePosition[i].x, pipePosition[i].y - pipeSize.height, 16, 32, pipeUpBitmap, pipeUpMaskBitmap, ESPERT_BLACK);
          drawBitmap(pipePosition[i].x, pipePosition[i].y + heightBetweenPipes, 16, 32, pipeDownBitmap, pipeDownMaskBitmap, ESPERT_BLACK);
        }
      }

      drawBitmap(birdPosition.x, birdPosition.y, 16, 16, birdBitmap[(int)birdFrame][(int)birdDegrees], birdMaskBitmap[(int)birdFrame][(int)birdDegrees], ESPERT_BLACK);

      if (gameMode == GAME_MODE_PLAY) {
        for (int i = 0; i < 3; i++) {
          if (scoreDigitImage[i]) {
            drawBitmap(scorePosition[i].x, scorePosition[i].y, 8, 16, scoreDigitImage[i], scoreDigitMaskImage[i], ESPERT_BLACK);
          }
        }
      } else if (gameMode == GAME_MODE_GAME_OVER) {
        drawBitmap(gameOverPosition.x, gameOverPosition.y, 64, 16, gameOverBitmap, gameOverMaskBitmap, ESPERT_BLACK);
        drawBitmap(25, 16 + scorePanelOffset, 80, 40, scorePanelBitmap, scorePanelMaskBitmap, ESPERT_BLACK);

        if (scorePanelMedalImage && scorePanelScore == score) {
          drawBitmap(28, 24 + scorePanelOffset, 32, 32, scorePanelMedalImage, NULL, ESPERT_BLACK);
        }

        if (isNewHighScore && scorePanelScore == scorePanelHighScore) {
          drawBitmap(63, 36 + scorePanelOffset, 16, 8, newBitmap, NULL, ESPERT_BLACK);
        }

        for (int i = 0; i < 3; i++) {
          int x = 78 + (i * 7);
          if (scorePanelScoreDigitImage[i]) {
            drawBitmap(x, 25 + scorePanelOffset, 8, 8, scorePanelScoreDigitImage[i], NULL, ESPERT_BLACK);
          }

          if (scorePanelHighScoreDigitImage[i]) {
            drawBitmap(x, 44 + scorePanelOffset, 8, 8, scorePanelHighScoreDigitImage[i], NULL, ESPERT_BLACK);
          }
        }
      }
      break;
  }

  drawBitmap(landOffset.x, landOffset.y, 128, 8, landBitmap, landMaskBitmap, ESPERT_BLACK);
  drawBitmap(landOffset.x - landSize.width, landOffset.y, 128, 8, landBitmap, landMaskBitmap, ESPERT_BLACK);

  if (gameMode == GAME_MODE_TITLE) {
    renderVolume(1, 1, ESPERT_BLACK);
    renderBattery(screenSize.width - batterySize.width - 1, 1, ESPERT_BLACK);
  }

  renderFPS(116, 52, 8, 8, -3, (const uint8_t*)numberBitmap, (const uint8_t*)numberMaskBitmap, ESPERT_BLACK, 102, 52);

  if (!isMenuEnabled) {
    espert->oled.update();
  }
}

void FlappyBird::renderHighScore() {
  drawBitmap(49, 38, 32, 8, highScoreBitmap, NULL, ESPERT_BLACK);
  String highScoreString = String(highScore);
  int numberwidth = 7;
  int x = (screenSize.width - (highScoreString.length() * (numberwidth - 1))) * 0.5f;
  int n = 0;

  for (int i = 0; i < highScoreString.length(); i++) {
    n = highScoreString.charAt(i) - '0';
    drawBitmap(x + (i * (numberwidth - 1)), 45, 8, 8, numberBitmap[n], numberMaskBitmap[n], ESPERT_BLACK);
  }
}

void FlappyBird::resetBird() {
  velocity = 0.0f;
  birdFrameDirection = 1;
  birdDegrees = 0.0f;
  isDropping = false;
  dropPosition = 0.0f;
  heightRatio = 0.0f;
  birdPosition.x = 21;
  birdPosition.y = (screenSize.height - birdSize.height) * 0.5f;
  isHit = false;
  isGameOver = false;
}

void FlappyBird::resetGame() {
  isAutoPlay = false;
  resetBird();
  isCheckHighScore = false;
  score = 0;
  setScoreImages();
  buttonDelay = 0.0f;
  pressedButton = BUTTON_NONE;
  landOffset.x = screenSize.width;

  memset(pipePosition, 0, sizeof(pipePosition));
  memset(isPipeVisibled, false, sizeof(isPipeVisibled));
  memset(isPipeEnabled, false, sizeof(isPipeEnabled));

  pipeRandomTime = pipeRandomFirstTime;
  gameOverVelocity = 0.0f;
  gameOverPosition.x = 32;
  gameOverPosition.y = 0;
  scorePanelOffset = 0.0f;
  scorePanelScore = 0.0f;
  scorePanelHighScore = 0.0f;
  isNewHighScore = false;
  memset(scorePanelHighScoreDigitImage, NULL, sizeof(scorePanelHighScoreDigitImage));
  memset(scorePanelScoreDigitImage, NULL, sizeof(scorePanelScoreDigitImage));
  scorePanelMedalImage = NULL;
  isButtonAllowed = true;
}

void FlappyBird::setScoreImages() {
  memset(&scoreDigitImage, NULL, sizeof(scoreDigitImage));
  memset(&scoreDigitMaskImage, NULL, sizeof(scoreDigitMaskImage));

  String string = longToString(score, 3, "0");
  int width = 6;
  int startDigit = 2;

  if (string.charAt(0) != '0') {
    width += 12;
    startDigit = 0;
  } else if (string.charAt(1) != '0') {
    width += 6;
    startDigit = 1;
  }

  int x = (screenSize.width - width) * 0.5f;

  for (int i = startDigit; i < 3; i++) {
    int n = string.charAt(i) - '0';
    scoreDigitImage[i] = numberScoreBitmap[n];
    scoreDigitMaskImage[i] = numberScoreMaskBitmap[n];
    scorePosition[i].x = x;
    x += 6;
  }
}

void FlappyBird::setScorePanelImages(const uint8_t* digitImage[3], int value) {
  String string = longToString(value, 3, "0");
  bool found = false;

  for (int i = 0; i < 3; i++) {
    digitImage[i] = NULL;
    int n = string.charAt(i) - '0';

    if (found || (!found && n > 0) || i == 2) {
      found = true;
      digitImage[i] = numberBitmap[n];
    }
  }
}

void FlappyBird::update() {
  updateGameTime();

  switch (gameMode) {
    case GAME_MODE_TITLE:
      updateBirdAnimation(false);

      if (birdPosition.y >= 20 + (gameMode == GAME_MODE_TITLE ? 5 : 0)) {
        velocity = initialVelocity * 0.7f;
      }

      titleTime += ((isVolumeChanged > 0.0f) ? 0.0f : elapsedTime);

      if (isGamepadEnabled && (pressedButton == BUTTON_UP || pressedButton == BUTTON_DOWN)) {
        if (pressedButton == BUTTON_UP) {
          increaseVolume();
        } else if (pressedButton == BUTTON_DOWN) {
          decreaseVolume();
        }

        titleTime = 0.0f;
      }

      if (titleTime >= 5000.0f) {
        isAutoPlay = true;
        changeGameMode(GAME_MODE_GET_READY);
      }
      break;

    case GAME_MODE_GET_READY:
      updateBirdAnimation(false);

      if (birdPosition.y >= 20 + (gameMode == GAME_MODE_TITLE ? 5 : 0)) {
        velocity = initialVelocity * 0.7f;
      }

      getReadyTime += elapsedTime;

      if (getReadyTime >= (isAutoPlay ? 3000.0f : 5000.0f)) {
        jump();
        changeGameMode(GAME_MODE_PLAY);
      }
      break;

    case GAME_MODE_PLAY:
      updateBirdAnimation(true);

      if (isGameOver) {
        changeGameMode(GAME_MODE_GAME_OVER);
      } else if (!isHit) {
        int nearestPipe = -1;
        float x = screenSize.width * 2.0f;

        for (int i = 0; i < numberOfPipes; i++) {
          if (isPipeVisibled[i]) {
            if (isPipeEnabled[i]) {
              if (isAutoPlay) {
                if (pipePosition[i].x < x) {
                  x = pipePosition[i].x;
                  nearestPipe = i;
                }
              }

              if (birdPosition.x + (birdSize.width * 0.5f) >= pipePosition[i].x + (pipeSize.width * 0.5f)) {
                if (birdPosition.y >= pipePosition[i].y - birdCollisionOffset.y && pipePosition[i].y <= pipePosition[i].y + heightBetweenPipes - birdSize.height + birdCollisionOffset.y) { // between two pipes
                  addScore(1);
                  isPipeEnabled[i] = false;
                  playSound(SOUND_SCORE);
                }
              }
            }

            pipePosition[i].x -= elapsedTime * landSpeed;
            if (pipePosition[i].x < -pipeSize.width) {
              isPipeVisibled[i] = false;
            }
          }
        }

        if (isAutoPlay) {
          if (isDropping && nearestPipe != -1 && isPipeVisibled[nearestPipe]) {
            float h = 0.73f + (random(0, 48) / 100.0f * ((random(0, 2) == 0) ? 1.0f : -1.0f));
            if (birdPosition.y > pipePosition[nearestPipe].y + (birdSize.height * h)) {
              jump();
            }
          }

          if (isDropping && birdPosition.y >= landPosition - (birdSize.height * 0.5f * (nearestPipe == -1 ? random(1, 4) : 1))) {
            jump();
          }
        }

        if (pipeRandomTime == 0.0f) {
          for (int i = 0; i < numberOfPipes; i++) {
            if (!isPipeVisibled[i]) {
              isPipeVisibled[i] = true;
              isPipeEnabled[i] = true;
              pipePosition[i].x = landOffset.x + (screenSize.width - landOffset.x);
              pipePosition[i].y = random(pipeRandomFrom, pipeRandomTo);
              pipeRandomTime = pipeRandomNextTime;
              break;
            }
          }
        } else {
          pipeRandomTime -= elapsedTime;

          if (pipeRandomTime <= 0.0f) {
            pipeRandomTime = 0.0f;
          }
        }
      }
      break;

    case GAME_MODE_GAME_OVER:
      gameOverVelocity += gravity * (elapsedTime / 2000.0f);
      gameOverPosition.y = constrain(gameOverPosition.y + gameOverVelocity, -64, landPosition);

      if (gameOverVelocity > 0.0f && gameOverPosition.y >= 0.0f) {
        gameOverPosition.y = 0.0f;

        if (isCheckHighScore && scorePanelOffset != maxScorePanelOffset) {
          isCheckHighScore = false;
          checkHighScore();
        }

        scorePanelOffset = lerp(0.2f, scorePanelOffset, 0.0f);

        if (scorePanelOffset <= 0.1f) {
          if (!isButtonAllowed) {
            isButtonAllowed = true;
            pressedButton = BUTTON_NONE;
          }

          if (scorePanelScore != score) {
            float speed = constrain(fabs(scorePanelScore - score), 5.0f, 999.0f);
            speed *= elapsedTime / 1000.0f;

            if (scorePanelScore < score) {
              scorePanelScore += speed;

              if (scorePanelScore > score) {
                scorePanelScore = score;
              }
            } else if (scorePanelScore > score) {
              scorePanelScore -= speed;

              if (scorePanelScore < score) {
                scorePanelScore = score;
              }
            }

            setScorePanelImages(scorePanelScoreDigitImage, scorePanelScore);

            if (isNewHighScore && scorePanelScore > scorePanelHighScore) {
              scorePanelHighScore = scorePanelScore;
              setScorePanelImages(scorePanelHighScoreDigitImage, scorePanelHighScore);
            }
          }
        }
      }

      if (isButtonAllowed && scorePanelScore == score) {
        gameOverTime += elapsedTime;

        if (gameOverTime >= 5000.0f) {
          changeGameMode(GAME_MODE_TITLE);
        }
      }
      break;
  }

  ESP.wdtFeed();
}

void FlappyBird::updateBirdAnimation(bool rotate) {
  isHit = isHitPipe();

  if (!isGameOver) {
    float speed = elapsedTime * birdAnimationSpeed;
    birdFrame += speed * birdFrameDirection;

    if (birdFrameDirection > 0 && birdFrame >= numberOfFrames) {
      birdFrame = numberOfFrames - 1;
      birdFrameDirection *= -1;
    } else if ( birdFrameDirection < 0 && birdFrame <= 0.0f) {
      birdFrame = 0.0f;
      birdFrameDirection *= -1;
    }

    velocity += gravity * (elapsedTime / 2000.0f);
    birdPosition.y = constrain(birdPosition.y + velocity, 0, landPosition);

    if (birdPosition.y == 0.0f) {
      velocity = 0.0f;
    }

    if (rotate) {
      if (velocity >= 0.0f) {
        if (!isDropping) {
          isDropping = true;
          dropPosition = birdPosition.y;
          heightRatio = (numberOfDegrees - birdFirstFrame) / (landPosition - (birdSize.height * 0.25f) - birdPosition.y);
        }

        birdDegrees = birdFirstFrame + ((birdPosition.y - dropPosition) * heightRatio);
      } else {
        birdDegrees -= speed * 2.0f;
      }

      birdDegrees = constrain(birdDegrees, birdFirstFrame, numberOfDegrees - 1);
    }

    if (birdDegrees == numberOfDegrees - 1) {
      birdFrame = 1.0f;

      if (!isGameOver && birdPosition.y == landPosition) {
        if (!isHit) {
          isHit = true;
          playSound(SOUND_DIE);
        }

        isGameOver = true;
        isButtonAllowed = false;
      }
    }

    if (!isHit) {
      speed = elapsedTime * landSpeed;
      landOffset.x -= speed;

      if (landOffset.x <= 0.0f) {
        landOffset.x = screenSize.width + landOffset.x;
      }
    }
  }
}
