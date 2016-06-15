// Breakout Simulator for ESPresso Lite

#include "Breakout.h"

using namespace breakout;

Breakout::Breakout() {
  // game
  gameMode = GAME_MODE_TITLE;
  titleBlinkTime = 0.0f;
  getReadyTime = 0.0f;
  missTime = 0.0f;
  miss = false;
  gameOverTimeOut = 0.0f;
  levelClearedTime = 0.0f;
  isReadyToPlay = false;
  titleTime = 0.0f;
  isAutoPlay = false;
  autoPlayPaddlePosition = -1.0f;
  autoPlayPaddleLerpSpeed = 0.02f;

  // paddle
  paddleExpandTimeOut = 0.0f;
  paddleType = 0.0f;
  isPaddleExpand = false;
  paddlePosition = {0.0f};
  isUpdatePaddlePosition = false;

  // ball
  ballLeft = 0;
  ballDigitImage = NULL;
  numberOfBalls = 0;
  memset(&isBallCollide, false, sizeof(isBallCollide));
  memset(&ballPosition, 0.0f, sizeof(ballPosition));
  memset(&ballDirection, 0, sizeof(ballDirection));
  memset(&ballSpeed, defaultBallSpeed, sizeof(ballSpeed));
  memset(&ballHitCount, 0, sizeof(ballHitCount));
  memset(&ballHitRowFirstTimeFlag, 0, sizeof(ballHitRowFirstTimeFlag));

  // brick
  memset(brickImage, NULL, sizeof(brickImage));
  memset(&brickScore, 0, sizeof(brickScore));
  memset(&brickPosition, 0, sizeof(brickPosition));
  memset(&brickPower, 0, sizeof(brickPower));
  memset(&rowFirstHitSpeed, 0.0f, sizeof(rowFirstHitSpeed));
  brickCount = 0;

  // level
  level = 0;
  memset(&currentLevelTable, 0, sizeof(currentLevelTable));
  memset(levelDigitImage, NULL, sizeof(levelDigitImage));
  rowCount = 0;
  isUpdateRowCounter = false;

  // score
  memset(scoreDigitImage, NULL, sizeof(scoreDigitImage));
  memset(highScoreDigitImage, NULL, sizeof(highScoreDigitImage));

  // bullet
  bulletTimeOut = 0.0f;
  memset(bulletImage, NULL, sizeof(bulletImage));
  memset(&isBulletCollide, false, sizeof(isBulletCollide));
  memset(&bulletPosition, 0.0f, sizeof(bulletPosition));
  bulletSpawnTime = 0.0f;
  isBulletEnabled = false;

  // item
  itemCount = 0;
  memset(itemImage, NULL, sizeof(itemImage));
  memset(&itemType, ITEM_TYPE_NONE, sizeof(itemType));
  memset(&itemPosition, 0.0f, sizeof(itemPosition));
  memset(&isItemCollide, false, sizeof(isItemCollide));
  addBallBlinkTime = 0.0f;
}

void Breakout::addBall(int value) {
  ballLeft = constrain(ballLeft + value, 0, 9);
  setBallImages();
}

void Breakout::addScore(int value) {
  score = constrain(score + value, 0, maxScore[gameIndex]);
  setScoreImages();
}

void Breakout::changeGameMode(int mode) {
  switch (mode) {
    case GAME_MODE_TITLE:
      resetGame();
      break;

    case GAME_MODE_TITLE_BLINK:
      titleBlinkTime = 0.0f;
      break;

    case GAME_MODE_GET_READY:
      if (gameMode == GAME_MODE_TITLE_BLINK) {
        if (isAutoPlay) {
          level = random(0, numberOfLevels);
        }

        loadLevel();
      }

      isReadyToPlay = false;
      autoPlayPaddlePosition = -1.0f;
      setScoreImages();
      setBallImages();
      getReadyTime = 0.0f;
      missTime = 0.0f;
      ballPosition[mainBall][0] = paddlePosition.x - (ballSize.width * 0.5f);
      ballPosition[mainBall][1] = paddlePosition.y - ballSize.height - 1;
      ballDirection[mainBall][0] = 1;
      ballDirection[mainBall][1] = -1;
      memset(&isBallCollide, false, sizeof(isBallCollide));
      break;

    case GAME_MODE_PLAY:
      playSound(SOUND_LAUNCH_BALL);
      getReadyTime = maxGetReadyTime;
      break;

    case GAME_MODE_MISS:
      miss = true;
      missTime = 0.0f;
      playSound(SOUND_MISS);
      checkHighScore();
      break;

    case GAME_MODE_LEVEL_CLEARED:
      levelClearedTime = 0.0f;
      memset(bulletImage, NULL, sizeof(bulletImage));
      memset(&isBulletCollide, false, sizeof(isBulletCollide));
      memset(itemImage, NULL, sizeof(itemImage));
      memset(&isItemCollide, false, sizeof(isItemCollide));
      checkHighScore();
      break;

    case GAME_MODE_GAME_OVER:
      gameOverTimeOut = 0.0f;
      pressedButton = BUTTON_NONE;
      addBallBlinkTime = 0.0f;
      break;
  }

  gameMode = mode;
}

void Breakout::checkButtons() {
  switch (gameMode) {
    case GAME_MODE_TITLE:
    case GAME_MODE_TITLE_BLINK:
      break;

    case GAME_MODE_GET_READY:
    case GAME_MODE_PLAY:
    case GAME_MODE_MISS:
    case GAME_MODE_LEVEL_CLEARED:
      if (isAutoPlay) {
        if (gameMode == GAME_MODE_PLAY) {
          if (autoPlayPaddlePosition == -1.0f || paddlePosition.x == autoPlayPaddlePosition) {
            autoPlayPaddleLerpSpeed = 0.02f + ((random(0, 10) / 100.0f));
            autoPlayPaddlePosition = ballPosition[mainBall][0];

            int paddleHalfSize = paddleSize[(int)paddleType].width * 0.5f;
            float leftPosition = screenRect[0] + paddleHalfSize;
            if (autoPlayPaddlePosition < leftPosition) {
              autoPlayPaddlePosition = leftPosition;
            } else {
              float rightPosition = screenRect[2] - paddleHalfSize + 1;
              if (autoPlayPaddlePosition > rightPosition) {
                autoPlayPaddlePosition = rightPosition;
              }
            }
          }

          paddlePosition.x = lerp(autoPlayPaddleLerpSpeed, paddlePosition.x, autoPlayPaddlePosition);
          isUpdatePaddlePosition = true;
        }
      } else {
        if (pressedButton == BUTTON_LEFT) {
          paddlePosition.x -= elapsedTime * paddleSpeed;
          isUpdatePaddlePosition = true;
        } else if (pressedButton == BUTTON_RIGHT) {
          paddlePosition.x += elapsedTime * paddleSpeed;
          isUpdatePaddlePosition = true;
        }
      }
      break;

    case GAME_MODE_GAME_OVER:
      break;
  }
}

void Breakout::checkHighScore() {
  if (!isAutoPlay && score > highScore) {
    highScore = score;
    writeHighScore();
  }
}

void Breakout::deleteItem(int i) {
  isItemCollide[i] = false;
  itemImage[i] = NULL;

  if (--itemCount < 0) {
    itemCount = 0;
  }
}

void Breakout::initGame() {
  gameIndex = GAME_BREAKOUT;
  readHighScore();
  readVolume();
  resetGame();
  changeGameMode(GAME_MODE_TITLE);
}

bool Breakout::isBackToMenuEnabled() {
  return (isMenuEnabled && gameMode == GAME_MODE_TITLE) ? true : false;
}

bool Breakout::isHitBrick(int x, int y, int width, int height, int directionX, int directionY, int ball) {
  bool isCollide = false;

  for (int row = 0; row < numberOfRows && !isCollide; row++) {
    for (int column = 0; column < numberOfColumns && !isCollide; column++) {
      if (brickImage[row][column]) {
        float w = (width + brickSize.width) * 0.5f;
        float h = (height + brickSize.height) * 0.5f;
        Point center = {x + (width * 0.5f), y + (height * 0.5f)};
        Point centerBrick = {brickPosition[row][column].x + (brickSize.width * 0.5f), brickPosition[row][column].y + (brickSize.height * 0.5f)};

        float dx = center.x - centerBrick.x;
        float dy = center.y - centerBrick.y;

        if (abs(dx) <= w && abs(dy) <= h) {
          isCollide = true;

          if (ball != -1) {
            float wy = w * dy;
            float hx = h * dx;

            if (wy > hx) {
              if (wy > -hx) { // hit top
                isBallCollide[ball][1] = true;
              } else { // hit left
                isBallCollide[ball][0] = true;
              }
            } else {
              if (wy > -hx) { // hit right
                isBallCollide[ball][0] = true;
              } else { // hit bottom
                isBallCollide[ball][1] = true;
              }
            }
          }

          playSound(SOUND_HIT_BLOCK);

          if (--brickPower[row][column] == 0) {
            brickImage[row][column] = NULL;
            int brickType = (currentLevelTable[row][column] / 10) - 1;
            addScore(brickScore[row]);

            if (ball != -1 && ballHitRowFirstTimeFlag[ball][row] == 0) {
              ballHitRowFirstTimeFlag[ball][row] = 1;
            }

            if (--brickCount <= 0) {
              brickCount = 0;
            }

            // spawn item
            int typ = (currentLevelTable[row][column] % 10);
            if (typ != ITEM_TYPE_NONE) {
              for (int i = 0; i < numberOfItems; i++) {
                if (itemImage[i] == NULL) {
                  itemImage[i] = itemBitmap[typ - 1];
                  itemType[i] = typ;
                  itemPosition[i].x = brickPosition[row][column].x + ((brickSize.width - itemSize.width) * 0.5f);
                  itemPosition[i].y = brickPosition[row][column].y + ((brickSize.height - itemSize.height) * 0.5f);
                  itemCount++;
                  break;
                }
              }
            }
          }
        }
      }
    }
  }

  return isCollide;
}

void Breakout::loadLevel() {
  numberOfBalls = 1;
  brickCount = 0;
  itemCount = 0;
  setLevelImages();
  rowCount = 0;
  isUpdateRowCounter = false;
  addBallBlinkTime = 0.0f;

  memcpy(&currentLevelTable, levelTable[level], sizeof(currentLevelTable));
  memset(&ballHitCount, 0, sizeof(ballHitCount));
  memset(&isBallCollide, false, sizeof(isBallCollide));
  memset(&itemType, ITEM_TYPE_NONE, sizeof(itemType));
  memset(bulletImage, NULL, sizeof(bulletImage));
  memset(&isBulletCollide, false, sizeof(isBulletCollide));
  memset(itemImage, NULL, sizeof(itemImage));
  memset(&isItemCollide, false, sizeof(isItemCollide));
  memset(&rowFirstHitSpeed, 0.0f, sizeof(rowFirstHitSpeed));
  memset(&ballHitRowFirstTimeFlag, 0, sizeof(ballHitRowFirstTimeFlag));

  int count = 0;
  int brickInRowCount[numberOfRows] = {0};

  for (int row = 0; row < numberOfRows; row++) {
    for (int column = 0; column < numberOfColumns; column++) {
      brickImage[row][column] = NULL;

      if (currentLevelTable[row][column] > 0) {
        brickCount++;
        brickInRowCount[row]++;
        int brickType = (currentLevelTable[row][column] / 10) - 1;
        int typ = (currentLevelTable[row][column] % 10);
        brickImage[row][column] = brickBitmap[brickType + (typ == 0 ? 0 : numberOfBrickType)];
        brickPosition[row][column].x = brickStartPosition.x + (column * (brickSize.width + brickGap.x));
        brickPosition[row][column].y = brickStartPosition.y + (row * (brickSize.height + brickGap.y));
        brickPower[row][column] = defaultBrickPower[brickType];
      }
    }

    if (brickInRowCount[row] > 0) {
      count++;
    }
  }

  for (int row = 0; row < numberOfRows; row++) {
    if (brickInRowCount[row] > 0) {
      brickScore[row] = count--;
      rowFirstHitSpeed[row] = count * 0.002f;
    }
  }

  for (int ball = 0; ball < maxBall; ball++) {
    ballSpeed[ball] = defaultBallSpeed;
  }
}

void Breakout::playLoop() {
  for (int ball = 0; ball < numberOfBalls; ball++) {
    for (int i = 0; i < 2; i++) {
      if (isBallCollide[ball][i]) {
        isBallCollide[ball][i] = false;
        ballDirection[ball][i] *= -1;
      }
    }

    if (ballDirection[ball][0] != 0 && ballDirection[ball][1] != 0) {
      float speed = elapsedTime * ballSpeed[ball];
      Point newPosition = {ballPosition[ball][0] + (ballDirection[ball][0] * speed), ballPosition[ball][1] + (ballDirection[ball][1] * speed)};

      // collision
      bool isFinished = false;
      while (!isFinished) {
        ballPosition[ball][0] += ballDirection[ball][0];
        ballPosition[ball][1] += ballDirection[ball][1];

        // paddle
        if (ballPosition[ball][1] >= paddlePosition.y - 1 && ballPosition[ball][1] <= paddlePosition.y + 1) {
          int paddleWidth = paddleSize[(int)paddleType].width;
          int paddleX = paddlePosition.x - (paddleWidth * 0.5f);

          if (ballPosition[ball][0] + ballSize.width - 1 >= paddleX && ballPosition[ball][0] < paddleX + paddleWidth) {
            isBallCollide[ball][1] = true;
            ballPosition[ball][1] = paddlePosition.y - 1.0f;
            playSound(SOUND_HIT_PADDLE);

            if (ballPosition[ball][0] + ballSize.width - 1 <= paddleX + 4) { // left corner
              if (ballDirection[ball][0] > 0) {
                isBallCollide[ball][0] = true;
              }
            } else if (ballPosition[ball][0] >= paddleX + paddleWidth - 4) { // right corner
              if (ballDirection[ball][0] < 0) {
                isBallCollide[ball][0] = true;
              }
            }
          }
        }

        // border
        for (int i = 0; i < 2 && !isBallCollide[ball][0] && !isBallCollide[ball][1]; i++) {
          if (ballPosition[ball][i] < screenRect[i]) { // left and top
            ballPosition[ball][i] = screenRect[i];
            isBallCollide[ball][i] = true;
            playSound(SOUND_HIT_BORDER);
          } else { // right and bottom
            float pos = screenRect[2 + i] - (i == 0 ? ballSize.width : ballSize.height) + 1;

            if (ballPosition[ball][i] > pos) {
              ballPosition[ball][i] = pos;
              isBallCollide[ball][i] = true;

              if (i == 0) { // right
                playSound(SOUND_HIT_BORDER);
              } else { // bottom
                if (numberOfBalls == 1) {
                  changeGameMode(GAME_MODE_MISS);
                } else if (numberOfBalls > 1) {
                  numberOfBalls--;
                  ballDirection[ball][0] = 0;
                  ballDirection[ball][1] = 0;
                }
              }
            }
          }
        }

        // brick
        if (!isBallCollide[ball][0] && !isBallCollide[ball][1]) {
          bool isCollide = isHitBrick(ballPosition[ball][0], ballPosition[ball][1], ballSize.width, ballSize.height, ballDirection[ball][0], ballDirection[ball][1], ball);

          // increase ball speed
          if (isCollide) {
            if (++ballHitCount[ball] == 4) { // after four hits
              ballSpeed[ball] += 0.005f;
            } else if (ballHitCount[ball] == 12) { // after twelve hits
              ballSpeed[ball] += 0.005f;
            } else { // first hit a brick in the row
              for (int i = 0; i < numberOfRows; i++) {
                if (ballHitRowFirstTimeFlag[ball][i] == 1) {
                  ballHitRowFirstTimeFlag[ball][i] = 2;
                  ballSpeed[ball] += rowFirstHitSpeed[i];
                }
              }
            }
          }
        }

        if (isBallCollide[ball][0] || isBallCollide[ball][1]) {
          isFinished = true;
        } else if ((ballDirection[ball][0] > 0 && ballPosition[ball][0] > newPosition.x) || (ballDirection[ball][0] < 0 && ballPosition[ball][0] < newPosition.x)) {
          isFinished = true;
        } else if ((ballDirection[ball][1] > 0 && ballPosition[ball][1] > newPosition.y) || (ballDirection[ball][1] < 0 && ballPosition[ball][1] < newPosition.y)) {
          isFinished = true;
        }
      }

      if (!isBallCollide[ball][0] && !isBallCollide[ball][1]) {
        ballPosition[ball][0] = newPosition.x;
        ballPosition[ball][1] = newPosition.y;
      } else {
        if (isAutoPlay && gameMode == GAME_MODE_PLAY) {
          autoPlayPaddlePosition = -1.0f;
        }
      }
    }
  }
}

void Breakout::playSound(int index) {
  if (!isAutoPlay && isSoundEnabled) {
    int frequency = 0;

    switch (index) {
      case SOUND_LAUNCH_BALL:
      case SOUND_HIT_PADDLE:
        frequency = 15;
        soundDuration = 20.0f;
        break;

      case SOUND_HIT_BORDER:
        frequency = 20;
        soundDuration = 20.0f;
        break;

      case SOUND_HIT_BLOCK:
        frequency = 25;
        soundDuration = 20.0f;
        break;

      case SOUND_MISS:
        frequency = 15;
        soundDuration = 428.0f;
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

void Breakout::pressButton() {
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

          if (gameMode == GAME_MODE_TITLE && isSoundEnabled && !isGamepadEnabled && pressedButton == BUTTON_RIGHT) {
            toggleVolume();
            titleTime = 0.0f;
          }
        } else {
          if (isVolumeChanged > 0.0f && isSoundEnabled) {
            if ((!isGamepadEnabled && i == BUTTON_RIGHT) || (isGamepadEnabled && (i == BUTTON_UP || i == BUTTON_DOWN))) {
              playSound(SOUND_VOLUME);
              titleTime = 0.0f;
            }
          } else if (isMenuEnabled && isGamepadEnabled && gameMode == GAME_MODE_TITLE && pressedButton == BUTTON_LEFT && i == BUTTON_LEFT) {
            isRequestingExit = true;
          } else if (i == pressedButton && ((isGamepadEnabled && (i == BUTTON_A || i == BUTTON_B)) || (!isGamepadEnabled && (i == BUTTON_LEFT || i == BUTTON_RIGHT)))) {
            if (isAutoPlay) {
              resetGame();
              changeGameMode(GAME_MODE_TITLE);
            } else {
              if (gameMode == GAME_MODE_TITLE) {
                changeGameMode(GAME_MODE_TITLE_BLINK);
              } else if (gameMode == GAME_MODE_GAME_OVER) {
                changeGameMode(GAME_MODE_TITLE);
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

void Breakout::render() {
  if (!isMenuEnabled) {
    espert->oled.clear(false);
  }

  espert->oled.setColor(ESPERT_WHITE);

  switch (gameMode) {
    case GAME_MODE_TITLE:
    case GAME_MODE_TITLE_BLINK:
      renderHighScore();
      renderVolume(0, 0);
      renderBattery(screenSize.width - batterySize.width, 0);
      renderMakerAsia(40, 54);

      if (gameMode == GAME_MODE_TITLE || (gameMode == GAME_MODE_TITLE_BLINK && ((int)titleBlinkTime % 500 < 250))) {
        espert->oled.drawBitmap(0, 10, 128, 32, breakoutBitmap, false);
      }
      break;

    case GAME_MODE_GET_READY:
    case GAME_MODE_PLAY:
    case GAME_MODE_MISS:
    case GAME_MODE_LEVEL_CLEARED:
    case GAME_MODE_GAME_OVER:
      if (gameMode != GAME_MODE_LEVEL_CLEARED) {
        for (int row = 0; row < rowCount; row++) {
          for (int column = 0; column < numberOfColumns; column++) {
            if (brickImage[row][column]) {
              espert->oled.drawBitmap(brickPosition[row][column].x, brickPosition[row][column].y, 16, 8, brickImage[row][column], false);
            }
          }
        }

        for (int i = 0; i < numberOfBullets; i++) {
          if (bulletImage[i]) {
            espert->oled.drawBitmap(bulletPosition[i].x, bulletPosition[i].y, 8, 8, bulletImage[i], false);
          }
        }
      }

      for (int i = 0; i < numberOfItems; i++) {
        if (itemImage[i]) {
          espert->oled.drawBitmap(itemPosition[i].x, itemPosition[i].y, 8, 8, itemImage[i], false);
        }
      }

      espert->oled.drawBitmap(paddlePosition.x - 16, paddlePosition.y, 32, 8, paddleBitmap[(int)paddleType], false);

      if (((gameMode == GAME_MODE_GET_READY && isReadyToPlay && miss) || (gameMode == GAME_MODE_GET_READY && !miss) || gameMode == GAME_MODE_PLAY || gameMode == GAME_MODE_LEVEL_CLEARED) || (gameMode == GAME_MODE_MISS && ((int)missTime % 500) < 250)) {
        for (int ball = 0; ball < numberOfBalls; ball++) {
          if (ballDirection[ball][0] != 0 && ballDirection[ball][1] != 0) {
            espert->oled.drawBitmap(ballPosition[ball][0], ballPosition[ball][1], 8, 8, ballBitmap, false);
          }
        }
      }

      // status bar
      espert->oled.drawBitmap(0, 0, 128, 8, statusBarBackgroundBitmap, false);

      if (gameMode == GAME_MODE_GET_READY) {
        if (getReadyTime <= 1500.0f) {
          espert->oled.drawBitmap(48, infoY, 32, 8, getReadyBitmap, false);
        } else if (getReadyTime >= 2000.0f && getReadyTime <= 2500.0f) {
          espert->oled.drawBitmap(62, infoY, 8, 8, numberBitmap[3], false);
        } else if (getReadyTime >= 3000.0f && getReadyTime <= 3500.0f) {
          espert->oled.drawBitmap(62, infoY, 8, 8, numberBitmap[2], false);
        } else if (getReadyTime >= 4000.0f && getReadyTime <= 4500.0f) {
          espert->oled.drawBitmap(62, infoY, 8, 8, numberBitmap[1], false);
        } else if (getReadyTime >= 5000.0f) {
          espert->oled.drawBitmap(60, infoY, 8, 8, goBitmap, false);
        }
      } else if (gameMode == GAME_MODE_GAME_OVER && ((int)gameOverTimeOut % 500) < 250) {
        espert->oled.drawBitmap(48, infoY, 32, 8, gameOverBitmap, false);
      } else if (gameMode == GAME_MODE_LEVEL_CLEARED && ((int)levelClearedTime % 500) < 250) {
        espert->oled.drawBitmap(37, infoY, 64, 8, levelClearedBitmap, false);
      }

      espert->oled.drawBitmap(0, 0, 8, 64, wallBitmap, false);
      espert->oled.drawBitmap(127, 0, 8, 64, wallBitmap, false);
      espert->oled.setColor(ESPERT_BLACK);
      espert->oled.drawBitmap(0, 0, 128, 8, isFPSVisibled ? statusBarBitmap[1] : statusBarBitmap[0], false);

      if (((int)addBallBlinkTime % 200) < 100) {
        espert->oled.drawBitmap(ballDigitPosition.x - (isFPSVisibled ? 14 : 0), ballDigitPosition.y, 8, 8, ballDigitImage, false);
      }

      for (int i = 0; i < 4; i++) {
        espert->oled.drawBitmap(112 - (isFPSVisibled ? 26 : 0) + (i * 4), 1, 8, 8, scoreDigitImage[i], false);

        if (i < 2) {
          espert->oled.drawBitmap(levelDigitPosition[i].x, levelDigitPosition[i].y, 8, 8, levelDigitImage[i], false);
        }
      }

      renderFPS(120, 1, 8, 8, -4, (const uint8_t*)numberBitmap, NULL, ESPERT_BLACK);
      break;
  }

  if (!isMenuEnabled) {
    espert->oled.update();
  }
}

void Breakout::renderHighScore() {
  drawBitmap(49, 41, 32, 8, highScoreBitmap);
  String highScoreString = String(highScore);
  int numberwidth = 5;
  int x = (screenSize.width - (highScoreString.length() * (numberwidth - 1))) * 0.5f;
  int n = 0;

  for (int i = 0; i < highScoreString.length(); i++) {
    n = highScoreString.charAt(i) - '0';
    drawBitmap(x + (i * (numberwidth - 1)), 48, 8, 8, numberBitmap[n]);
  }
}

void Breakout::resetGame() {
  isAutoPlay = false;
  autoPlayPaddlePosition = -1.0f;
  autoPlayPaddleLerpSpeed = 0.02f;
  titleTime = 0.0f;
  titleBlinkTime = 0.0f;
  getReadyTime = 0.0f;
  missTime = 0.0f;
  levelClearedTime = 0.0f;
  gameOverTimeOut = 0.0f;
  paddleType = 0.0f;
  paddleExpandTimeOut = 0.0f;
  isPaddleExpand = false;
  isUpdatePaddlePosition = false;
  paddlePosition.x = 128.0f * 0.5f;
  paddlePosition.y = (float)(screenRect[3] - paddleSize[(int)paddleType].height) + 1.0f;
  ballLeft = newGameBallLeft;
  setBallImages();
  level = 0;
  setLevelImages();
  score = 0;
  setScoreImages();
  buttonDelay = 0.0f;
  pressedButton = BUTTON_NONE;
  isBulletEnabled = false;
  miss = true;
  bulletTimeOut = maxBulletTimeOut;

  for (int ball = 0; ball < maxBall; ball++) {
    ballSpeed[ball] = defaultBallSpeed;
    ballPosition[ball][0] = paddlePosition.x - (ballSize.width * 0.5f);
    ballPosition[ball][1] = paddlePosition.y - ballSize.height - 1;
    ballDirection[ball][0] = (ball == mainBall) ? 1 : 0;
    ballDirection[ball][1] = (ball == mainBall) ? -1 : 0;
  }

  memset(&isBallCollide, false, sizeof(isBallCollide));
  memset(&ballHitCount, 0, sizeof(ballHitCount));
  memset(&isButtonPressed, false, sizeof(isButtonPressed));
  memset(bulletImage, NULL, sizeof(bulletImage));
  memset(&bulletPosition, 0.0f, sizeof(bulletPosition));
  memset(&isBulletCollide, false, sizeof(isBulletCollide));
  memset(itemImage, NULL, sizeof(itemImage));
  memset(&isItemCollide, false, sizeof(isItemCollide));

  readHighScore();
  setHighScoreImages();
}

void Breakout::setBallImages() {
  String string = longToString((ballLeft > 0) ? ballLeft : 0, 1, "0");
  ballDigitImage = numberBitmap[string.charAt(0) - '0'];
}

void Breakout::setHighScoreImages() {
  String string = longToString(highScore, 4, "0");

  for (int i = 0; i < 4; i++) {
    highScoreDigitImage[i] = numberBitmap[string.charAt(i) - '0'];
  }
}

void Breakout::setLevelImages() {
  String string = longToString(level + 1, 2, "0");

  for (int i = 0; i < 2; i++) {
    levelDigitImage[i] = numberBitmap[string.charAt(i) - '0'];
  }
}

void Breakout::setScoreImages() {
  String string = longToString(score, 4, "0");

  for (int i = 0; i < 4; i++) {
    scoreDigitImage[i] = numberBitmap[string.charAt(i) - '0'];
  }
}

void Breakout::update() {
  updateGameTime();
  checkButtons();

  switch (gameMode) {
    case GAME_MODE_TITLE:
      titleTime += ((isVolumeChanged > 0.0f) ? 0.0f : elapsedTime);

      if (isSoundEnabled && (isGamepadEnabled && (pressedButton == BUTTON_UP || pressedButton == BUTTON_DOWN))) {
        if (pressedButton == BUTTON_UP) {
          increaseVolume();
        } else if (pressedButton == BUTTON_DOWN) {
          decreaseVolume();
        }

        titleTime = 0.0f;
      }

      if (titleTime >= 5000.0f) {
        isAutoPlay = true;
        changeGameMode(GAME_MODE_TITLE_BLINK);
        break;
      }
      break;

    case GAME_MODE_TITLE_BLINK:
      if (titleBlinkTime < maxTitleBlinkTime) {
        titleBlinkTime += elapsedTime;

        if (titleBlinkTime >= maxTitleBlinkTime) {
          changeGameMode(GAME_MODE_GET_READY);
        }
      }
      break;

    case GAME_MODE_GET_READY:
      if (getReadyTime < maxGetReadyTime) {
        getReadyTime += elapsedTime;

        if (rowCount < numberOfRows && getReadyTime <= 1500.0f) {
          if ((int)getReadyTime % 300 < 150) {
            if (!isUpdateRowCounter) {
              isUpdateRowCounter = true;
              rowCount++;
            }
          } else {
            isUpdateRowCounter = false;
          }
        } else if (!isReadyToPlay && getReadyTime > 1500.0f) {
          isReadyToPlay = true;

          if (miss) {
            if (ballLeft > 0) {
              ballLeft--;
            }

            setBallImages();
            miss = false;
          }
        } else if (getReadyTime >= maxGetReadyTime) {
          changeGameMode(GAME_MODE_PLAY);
        }
      }
      break;

    case GAME_MODE_PLAY:
      playLoop();
      break;

    case GAME_MODE_MISS:
      if (missTime < maxMissTime) {
        missTime += elapsedTime;

        if (missTime >= maxMissTime) {
          if (isAutoPlay) {
            changeGameMode(GAME_MODE_TITLE);
          } else {
            changeGameMode((ballLeft == 0) ? GAME_MODE_GAME_OVER : GAME_MODE_GET_READY);
          }
        }
      }
      break;

    case GAME_MODE_LEVEL_CLEARED:
      if (levelClearedTime < maxLevelClearedTime) {
        levelClearedTime += elapsedTime;

        if (levelClearedTime >= maxLevelClearedTime) {
          if (isAutoPlay) {
            changeGameMode(GAME_MODE_TITLE);
          } else {
            if (++level >= numberOfLevels) {
              level = 0;
            }

            loadLevel();
            changeGameMode(GAME_MODE_GET_READY);
          }
        }
      }
      break;

    case GAME_MODE_GAME_OVER:
      if (gameOverTimeOut < maxGameOverTimeOut) {
        gameOverTimeOut += elapsedTime;

        if (gameOverTimeOut >= maxGameOverTimeOut) {
          changeGameMode(GAME_MODE_TITLE);
        }
      }
      break;
  }

  if (gameMode == GAME_MODE_GET_READY || gameMode == GAME_MODE_PLAY || gameMode == GAME_MODE_MISS || gameMode == GAME_MODE_LEVEL_CLEARED) {
    updatePaddle();

    if (addBallBlinkTime > 0.0f) {
      addBallBlinkTime -= elapsedTime;

      if (addBallBlinkTime <= 0.0f) {
        addBallBlinkTime = 0.0f;
      }
    }

    if (gameMode == GAME_MODE_PLAY && brickCount == 0 && itemCount == 0) {
      changeGameMode(GAME_MODE_LEVEL_CLEARED);
    }
  }

  ESP.wdtFeed();
}

void Breakout::updatePaddle() {
  if (isPaddleExpand) {
    if (paddleType != numberOfPaddleTypes - 1) {
      isUpdatePaddlePosition = true;
      paddleType += elapsedTime * paddleExpandSpeed;

      if (paddleType >= numberOfPaddleTypes - 1) {
        paddleType = numberOfPaddleTypes - 1;
        paddleExpandTimeOut = 0.0f;
      }
    } else if (gameMode == GAME_MODE_PLAY) {
      paddleExpandTimeOut += elapsedTime;

      if (paddleExpandTimeOut >= maxPaddleExpandTimeOut) {
        paddleExpandTimeOut = 0.0f;
        isPaddleExpand = false;
      }
    }
  } else {
    if (paddleType != 0.0f) {
      isUpdatePaddlePosition = true;
      paddleType -= elapsedTime * paddleExpandSpeed;

      if (paddleType <= 0.0f) {
        paddleType = 0.0f;
        paddleExpandTimeOut = 0.0f;
      }
    }
  }

  int paddleHalfSize = paddleSize[(int)paddleType].width * 0.5f;

  if (isUpdatePaddlePosition) {
    isUpdatePaddlePosition = false;
    float leftPosition = screenRect[0] + paddleHalfSize;

    if (paddlePosition.x < leftPosition) {
      paddlePosition.x = leftPosition;
    } else {
      float rightPosition = screenRect[2] - paddleHalfSize + 1;
      if (paddlePosition.x > rightPosition) {
        paddlePosition.x = rightPosition;
      }
    }
  }

  if (gameMode == GAME_MODE_GET_READY || gameMode == GAME_MODE_LEVEL_CLEARED) {
    ballPosition[mainBall][0] = paddlePosition.x - (ballSize.width * 0.5f);
    ballPosition[mainBall][1] = paddlePosition.y - ballSize.height - 1;
  }

  if (gameMode == GAME_MODE_PLAY) {
    // bullet
    if (isBulletEnabled) {
      bulletSpawnTime += elapsedTime;

      if (bulletSpawnTime >= maxBulletSpawnTime) {
        bulletSpawnTime = 0.0f;
        int emptyCount = 0;
        int emptyIndex[2] = {0};
        for (int i = 0; i < numberOfBullets && emptyCount < 2; i++) {
          if (bulletImage[i] == NULL) {
            emptyIndex[emptyCount] = i;
            emptyCount++;
          }
        }

        if (emptyCount == 2) {
          for (int i = 0; i < emptyCount; i++) {
            bulletImage[emptyIndex[i]] = bulletBitmap[i];
            bulletPosition[emptyIndex[i]].x = paddlePosition.x + ((i == 0) ? -paddleHalfSize + 2 : (paddleHalfSize - (bulletSize.width * 0.5f) - 3.0f));
            bulletPosition[emptyIndex[i]].y = paddlePosition.y - 4.0f;
          }
        }
      }

      bulletTimeOut += elapsedTime;

      if (bulletTimeOut >= maxBulletTimeOut) {
        bulletTimeOut = 0.0f;
        isBulletEnabled = false;
      }
    }

    for (int i = 0; i < numberOfBullets; i++) {
      if (bulletImage[i]) {
        if (isBulletCollide[i]) {
          isBulletCollide[i] = false;
          bulletImage[i] = NULL;
        } else {
          float newBulletPosition = bulletPosition[i].y - (elapsedTime * bulletSpeed);
          float topPosition = screenRect[1] - bulletSize.height;

          while ((int)bulletPosition[i].y != (int)newBulletPosition && !isBulletCollide[i]) {
            bulletPosition[i].y -= 1.0f;

            if ((int)bulletPosition[i].y <= topPosition) {
              bulletPosition[i].y = topPosition;
              isBulletCollide[i] = true;
            }

            if (!isBulletCollide[i]) {
              isBulletCollide[i] = isHitBrick(bulletPosition[i].x, bulletPosition[i].y, bulletSize.width, bulletSize.height, 0, -1, -1);
            }
          }

          if (!isBulletCollide[i]) {
            bulletPosition[i].y = newBulletPosition;
          }
        }
      }
    }

    // item
    for (int i = 0; i < numberOfItems; i++) {
      if (itemImage[i]) {
        if (isItemCollide[i]) {
          deleteItem(i);

          switch (itemType[i]) {
            case ITEM_TYPE_EXPAND:
              isPaddleExpand = true;
              paddleExpandTimeOut = 0.0f;
              bulletSpawnTime = maxBulletSpawnTime;
              break;

            case ITEM_TYPE_BALL:
              addBall(3);
              addBallBlinkTime = maxAddBallBlinkTime;
              break;

            case ITEM_TYPE_BULLET:
              isBulletEnabled = true;
              bulletTimeOut = 0.0f;
              bulletSpawnTime = maxBulletSpawnTime;
              break;
          }
        } else {
          float newItemPosition = itemPosition[i].y + (elapsedTime * itemSpeed);

          while ((int)itemPosition[i].y != (int)newItemPosition && !isItemCollide[i]) {
            itemPosition[i].y += 1.0f;
            int y = (int)itemPosition[i].y + itemSize.height - 1;

            if (y == paddlePosition.y - 1) {
              int paddleWidth = paddleSize[(int)paddleType].width;
              int paddleX = paddlePosition.x - (paddleWidth * 0.5f);

              if ((int)itemPosition[i].x + itemSize.width - 1 >= paddleX && (int)itemPosition[i].x < paddleX + paddleWidth) {
                isItemCollide[i] = true;
                playSound(SOUND_HIT_PADDLE);
              }
            }

            if (!isItemCollide[i] && itemPosition[i].y > screenRect[3]) { // miss item
              deleteItem(i);
            }
          }

          if (!isItemCollide[i] && itemImage[i]) {
            itemPosition[i].y = newItemPosition;
          }
        }
      }
    }
  }
}
