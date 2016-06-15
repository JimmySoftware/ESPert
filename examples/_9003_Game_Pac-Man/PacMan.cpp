// Pac-Man Simulator for ESPresso Lite

#include "PacMan.h"

using namespace pacMan;

PacMan::PacMan() {
  // game
  screenCenter = {screenSize.width * 0.5f, screenSize.height * 0.5f};
  gameMode = GAME_MODE_TITLE;
  readyTime = 0.0f;
  readyPosition = {0.0f, 0.0f};
  gameSpeed = 0.0f;
  level = 0;
  successTime = 0.0f;
  gameOverTime = 0.0f;
  gameOverPosition = {0.0f, 0.0f};
  titleTime = 0.0f;
  isAutoPlay = false;

  // score
  bool isAwardExtraLife = false;

  // item
  itemTile = {0, 0};
  itemPosition = {0.0f, 0.0f};
  itemIndex = 0;
  itemPointString = "";
  itemPointSize = {0, 0};
  itemPointPosition = {0.0f, 0.0f};
  itemVisibledTime = 0.0f;

  // world
  worldPosition = {0.0f, 0.0f};
  memset(&world, 0, sizeof(world));
  worldStartPosition = 0.0f;
  availabledTile = {0, 0};

  // pac-man
  pacManPosition = {0.0f, 0.0f};
  pacManDirection = DIRECTION_LEFT;
  pacManNextDirection = pacManDirection;
  pacManFrame = 0.0f;
  isPacManStoped = false;
  pacManTile = {0, 0};
  pacManLastTile = {0, 0};
  numberOfPacManLives = startPacManLives;
  maxPowerPelletTime = 10000.0f;
  isPacManVisibled = false;
  powerPelletTime = 0.0f;
  dotCounter = 0;
  numberOfDots = 0;
  pacManDieTime = 0.0f;
  pacManDieFrame = 0.0f;
  isPacManDisappeared = false;
  pacManIncreseSpeed = 0.0f;
  pacManIncreseSpeedTime = 0.0f;

  // ghost
  memset(&ghostPosition, 0.0f, sizeof(ghostPosition));
  memset(&ghostDirection, DIRECTION_NONE, sizeof(ghostDirection));
  memset(&ghostNextDirection, DIRECTION_NONE, sizeof(ghostNextDirection));
  memset(&ghostFrame, 0.0f, sizeof(ghostFrame));
  memset(&isGhostStoped, false, sizeof(isGhostStoped));
  memset(&ghostTile, 0, sizeof(ghostTile));
  memset(&ghostLastTile, 0, sizeof(ghostLastTile));
  memset(&isGhostVisibled, false, sizeof(isGhostVisibled));
  memset(&ghostWaitTime, 0.0f, sizeof(ghostWaitTime));
  memset(&ghostRandomTime, 0.0f, sizeof(ghostRandomTime));
  memset(&isRandomGhostPath, false, sizeof(isRandomGhostPath));
  memset(&isRunaway, false, sizeof(isRunaway));
  memset(&ghostState, GHOST_STATE_WAIT, sizeof(ghostState));
  memset(&ghostPath, 0, sizeof(ghostPath));
  memset(&ghostNumberOfPaths, 0, sizeof(ghostNumberOfPaths));
  memset(&ghostCurrentPath, -1, sizeof(ghostCurrentPath));
  memset(&isGhostEdible, false, sizeof(isGhostEdible));
  ghostEdiblePointIndex = 0;
  ghostEdiblePointVisibledTime = 0.0f;
  ghostEdiblePointString = "";
  ghostEdiblePointSize = {0, 0};
  ghostEdiblePointPosition = {0.0f, 0.0f};
  ghostEdiblePointGhost = -1;
}

void PacMan::addDotCounter(int value) {
  dotCounter += value;

  if (itemVisibledTime == 0.0f && (dotCounter == 70 || dotCounter == 170)) { // spawn item
    itemVisibledTime = 10000.0f;
  }

  if (dotCounter >= numberOfDots) {
    changeGameMode(GAME_MODE_SUCCESS);
  }
}

void PacMan::addGhostPath(int i, Tile tile) {
  if (ghostNumberOfPaths[i] < ghostMaxPaths - 1) {
    ghostPath[i][ghostNumberOfPaths[i]++] = tile;
  }
}

void PacMan::addScore(unsigned long value) {
  score = constrain(score + value, 0l, maxScore[gameIndex]);

  if (!isAwardExtraLife && score >= 10000l) {
    isAwardExtraLife = true;
    numberOfPacManLives++;
  }

  if (score > highScore) {
    highScore = score;
  }
}

void PacMan::changeGameMode(int mode) {
  switch (mode) {
    case GAME_MODE_TITLE:
      writeHighScore();
      resetGame();
      titleTime = 0.0f;
      pacManDirection = DIRECTION_LEFT;
      pacManPosition = {screenSize.width, 53.0f};
      break;

    case GAME_MODE_READY:
      writeHighScore();
      break;

    case GAME_MODE_PLAY:
      readyTime = maxReadyTime;
      break;

    case GAME_MODE_DIE:
      playSound(SOUND_DIE);
      pacManDieTime = 0.0f;
      pacManDieFrame = 0.0f;
      isPacManDisappeared = false;
      break;

    case GAME_MODE_SUCCESS:
      successTime = 0.0f;
      break;

    case GAME_MODE_GAME_OVER:
      gameOverTime = 0.0f;
      break;
  }

  gameMode = mode;
}

void PacMan::chasePacMan(int i) {
  if (powerPelletTime == 0.0f && pathFinding(i, pacManTile)) {
    readGhostPath(i);
  }
}

void PacMan::eat(Tile tile) {
  if (gameMode != GAME_MODE_READY && gameMode != GAME_MODE_GAME_OVER && gameMode != GAME_MODE_SUCCESS && gameMode != GAME_MODE_DIE) {
    if (tile.row >= 0 && tile.row < numberOfRows && tile.column >= 0 && tile.column < numberOfColumns) {
      if (itemVisibledTime > 0.0f && tile.row == itemTile.row && tile.column == itemTile.column) { // eat item
        itemVisibledTime = -2000.0f;
        addScore(itemPoint[item[itemIndex]]);
        playSound(SOUND_EAT_ITEM);
        resetPacManIncreseSpeed();
      }

      for (int i = 0; i < numberOfGhosts; i++) {
        if (isCollideGhost(i)) {
          if (isGhostEdible[i]) { // eat ghost
            if (ghostEdiblePointVisibledTime == 0.0f) {
              eatGhost(i);
              break;
            }
          } else if (ghostState[i] != GHOST_STATE_DIE) {
            changeGameMode(GAME_MODE_DIE);
            break;
          }
        }
      }

      if (gameMode != GAME_MODE_READY && gameMode != GAME_MODE_GAME_OVER && gameMode != GAME_MODE_SUCCESS && gameMode != GAME_MODE_DIE) {
        if (world[tile.row][tile.column] == TILE_TYPE_DOT || world[tile.row][tile.column] == TILE_TYPE_POWER_PELLET) {
          if (world[tile.row][tile.column] == TILE_TYPE_DOT) {
            addScore(10);
            playSound(SOUND_EAT_DOT);
            addDotCounter(1);
          } else if (world[tile.row][tile.column] == TILE_TYPE_POWER_PELLET) {
            addScore(50);
            playSound(SOUND_EAT_POWER_PELLET);
            eatPowerPellet();
          }

          world[tile.row][tile.column] = TILE_TYPE_SPACE;
          resetPacManIncreseSpeed();
        }
      }
    }
  }
}

void PacMan::eatGhost(int i) {
  ghostEdiblePointVisibledTime = 1000.0f;
  ghostEdiblePointString = String(ghostEdiblePoint[ghostEdiblePointIndex]);
  ghostEdiblePointSize = {(ghostEdiblePointString.length() * (numberSize.width - 1)) - 1, numberSize.height};
  ghostEdiblePointPosition = getPositionFromTile(pacManTile, ghostEdiblePointSize);
  addScore(ghostEdiblePoint[ghostEdiblePointIndex]);
  playSound(SOUND_EAT_GHOST);
  ghostEdiblePointIndex++;
  ghostEdiblePointGhost = i;
  ghostDie(i);
}

void PacMan::eatPowerPellet() {
  addDotCounter(1);
  powerPelletTime = maxPowerPelletTime;
  ghostEdiblePointIndex = 0;

  for (int i = 0; i < numberOfGhosts; i++) {
    if (ghostState[i] != GHOST_STATE_DIE) {
      isGhostEdible[i] = true;
      setRandomGhostPath(i, true);
    }
  }
}

void PacMan::exitAutoPlay() {
  readHighScore();
  changeGameMode(GAME_MODE_TITLE);
}

int PacMan::getDirection(Tile fromTile, Tile toTile) {
  int direction = DIRECTION_NONE;

  if (toTile.row == fromTile.row) {
    if (toTile.column == fromTile.column - 1) {
      direction = DIRECTION_LEFT;
    } else if (toTile.column == fromTile.column + 1) {
      direction = DIRECTION_RIGHT;
    }
  } else if (toTile.column == fromTile.column) {
    if (toTile.row == fromTile.row - 1) {
      direction = DIRECTION_UP;
    } else if (toTile.row == fromTile.row + 1) {
      direction = DIRECTION_DOWN;
    }
  }

  return direction;
}

Game::Point PacMan::getPositionFromTile(Tile tile, Size size) {
  Point pos = {0, 0};
  pos.y = round((tile.row * tileSize.height) + ((tileSize.height - size.height) * 0.5f));
  pos.x = round((tile.column * tileSize.width) + ((tileSize.width - size.width) * 0.5f));
  return pos;
}

PacMan::Tile PacMan::getTileFromPosition(Point pos, Size size) {
  Tile tile = {0, 0};
  tile.row = (pos.y - ((tileSize.height - size.height) * 0.5f)) / tileSize.height;
  tile.column = (pos.x - ((tileSize.width - size.width) * 0.5f)) / tileSize.width;
  return tile;
}

void PacMan::ghostDie(int i) {
  ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
  ghostState[i] = GHOST_STATE_DIE;
  isGhostEdible[i] = false;

  if (pathFinding(i, {13, 10}, false)) {
    readGhostPath(i);
  }
}

void PacMan::initGame() {
  gameIndex = GAME_PAC_MAN;
  readHighScore();
  readVolume();
  resetGame();
  changeGameMode(GAME_MODE_TITLE);
  titleTime = 250.0f;
}

bool PacMan::isBackToMenuEnabled() {
  return (isMenuEnabled && gameMode == GAME_MODE_TITLE && titleTime >= 250.0f && titleTime < 5250.0f) ? true : false;
}

bool PacMan::isCollideGhost(int i) {
  float offset = 2.0f;
  return (pacManPosition.x + offset < ghostPosition[i].x + ghostSize.width - 1 - offset &&
          pacManPosition.x + pacManSize.width - 1 - offset > ghostPosition[i].x + offset &&
          pacManPosition.y + offset < ghostPosition[i].y + ghostSize.height - 1 - offset &&
          pacManPosition.y + pacManSize.height - 1 - offset > ghostPosition[i].y + offset);
}

bool PacMan::isGhostPath(int i) {
  bool found = (ghostNumberOfPaths[i] > 0) ? true : false;

  if (found) {
    int direction = ghostDirection[i];

    if ((direction == DIRECTION_LEFT && ghostTile[i].column < ghostPath[i][ghostCurrentPath[i]].column) ||
        (direction == DIRECTION_RIGHT && ghostTile[i].column > ghostPath[i][ghostCurrentPath[i]].column) ||
        (direction == DIRECTION_UP && ghostTile[i].row < ghostPath[i][ghostCurrentPath[i]].row) ||
        (direction == DIRECTION_DOWN && ghostTile[i].row > ghostPath[i][ghostCurrentPath[i]].row)) {
      ghostTile[i] = ghostPath[i][ghostCurrentPath[i]];
      bool found = readGhostPath(i);

      if (!found || ghostDirection[i] != direction) {
        ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
      }
    }
  }

  return found;
}

bool PacMan::isJustMoveToNewTile(Tile currentTile, Tile lastTile) {
  return (currentTile.row != lastTile.row || currentTile.column != lastTile.column) ? true : false;
}

bool PacMan::isTileAvailabled(int direction, Tile tile, bool isDoorTileEnabled) {
  bool availabled = false;
  int row = tile.row;
  int column = tile.column;
  availabledTile = tile;

  if (direction == DIRECTION_LEFT) {
    column--;
    availabled = (tile.column > 0 && (world[tile.row][column] <= TILE_TYPE_POWER_PELLET || (!isDoorTileEnabled && world[tile.row][column] == TILE_TYPE_DOOR)));
  } else if (direction == DIRECTION_RIGHT) {
    column++;
    availabled = (tile.column < numberOfColumns - 1 && (world[tile.row][column] <= TILE_TYPE_POWER_PELLET || (!isDoorTileEnabled && world[tile.row][column] == TILE_TYPE_DOOR)));
  } else if (direction == DIRECTION_UP) {
    row--;
    availabled = (tile.row > 0 && tile.column >= 0 && tile.column < numberOfColumns && (world[row][tile.column] <= TILE_TYPE_POWER_PELLET || (!isDoorTileEnabled && world[row][tile.column] == TILE_TYPE_DOOR)));
  } else if (direction == DIRECTION_DOWN) {
    row++;
    availabled = (tile.row < numberOfRows - 1 && tile.column >= 0 && tile.column < numberOfColumns && (world[row][tile.column] <= TILE_TYPE_POWER_PELLET || (!isDoorTileEnabled && world[row][tile.column] == TILE_TYPE_DOOR)));
  }

  if (availabled) {
    availabledTile = {row, column};
  }

  return availabled;
}

bool PacMan::isWallTile(Tile tile, bool isDoorTileEnabled) {
  bool isWall = false;

  if (tile.row >= 0 && tile.row < numberOfRows && tile.column >= 0 && tile.column < numberOfColumns) {
    if (world[tile.row][tile.column] > TILE_TYPE_POWER_PELLET) {
      isWall = (world[tile.row][tile.column] == TILE_TYPE_DOOR) ? isDoorTileEnabled : true;
    }
  }

  return isWall;
}

void PacMan::newLevel() {
  memcpy(&world, &initialWorld, sizeof(world));
  numberOfDots = 0;
  for (byte row = 0; row < numberOfRows; row++) {
    for (byte column = 0; column < numberOfColumns; column++) {
      if (world[row][column] == TILE_TYPE_DOT || world[row][column] == TILE_TYPE_POWER_PELLET) {
        numberOfDots++;
      }
    }
  }

  gameSpeed = constrain(25.0f + (level * 2.0f), 25.0f, 37.0f) / 1000.0f;
  maxPowerPelletTime = constrain(10000.0f - (level * 2000.0f), 3000.0f, 10000.0f);
  dotCounter = 0;

  // item
  itemVisibledTime = 0.0f;
  itemIndex = item[(level >= maxItems - 1) ? maxItems - 1 : level];
  itemPointString = String(itemPoint[itemIndex]);
  itemPointSize = {(itemPointString.length() * (numberSize.width - 1)) - 1, numberSize.height};
  itemPointPosition = getPositionFromTile(itemTile, itemPointSize);

  restartLevel();
}

bool PacMan::pathFinding(int i, Tile targetTile, bool isDoorTileEnabled) {
  bool finished = false;
  int tryCounter = ghostMaxPaths * 3;
  Tile currentTile = ghostTile[i];
  Tile lastTile = currentTile;
  availabledTile = currentTile;
  resetGhostPath(i);

  while (!finished) {
    int direction[numberOfDirections] = {DIRECTION_NONE};

    if (currentTile.row <= targetTile.row) {
      if (currentTile.column <= targetTile.column) {
        memcpy(&direction, &directionOrderUpperLeft, sizeof(direction));
      } else {
        memcpy(&direction, &directionOrderUpperRight, sizeof(direction));
      }
    } else {
      if (currentTile.column <= targetTile.column) {
        memcpy(&direction, &directionOrderLowerLeft, sizeof(direction));
      } else {
        memcpy(&direction, &directionOrderLowerRight, sizeof(direction));
      }
    }

    for (int n = 0; n < numberOfDirections; n++) {
      if (isTileAvailabled(direction[n], currentTile, isDoorTileEnabled) && !(availabledTile.row == lastTile.row && availabledTile.column == lastTile.column)) {
        lastTile = currentTile;
        currentTile = availabledTile;
        addGhostPath(i, currentTile);
        break;
      }
    }

    if ((currentTile.row == targetTile.row && currentTile.column == targetTile.column) || ghostNumberOfPaths[i] >= ghostMaxPaths - 1 || --tryCounter <= 0) {
      finished = true;
    }

    ESP.wdtFeed();
  }

  return (ghostNumberOfPaths[i] > 0) ? true : false;
}

void PacMan::playSound(int index) {
  if (!isAutoPlay && isSoundEnabled && isSoundInterruptEnabled) {
    nextSound = SOUND_NONE;
    nextSoundDelay = 0.0f;
    int frequency = 0;

    switch (index) {
      case SOUND_EAT_DOT:
        frequency = 20;
        soundDuration = 20.0f;
        break;

      case SOUND_EAT_ITEM:
      case SOUND_EAT_GHOST:
      case SOUND_EAT_POWER_PELLET:
      case SOUND_DIE:
      case SOUND_DISAPPEAR:
      case SOUND_VOLUME:
        frequency = 25;
        soundDuration = 100.0f;
        isSoundInterruptEnabled = false;
        break;

      default:
        frequency = 0;
        soundDuration = 0.0f;
        break;
    }

    espert->buzzer.on((int)constrain(frequency * volume, 0.0f, 25.0f));
  }
}

void PacMan::pressButton() {
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

          if (!isAutoPlay && (gameMode == GAME_MODE_READY || gameMode == GAME_MODE_PLAY) && (pressedButton == BUTTON_LEFT || pressedButton == BUTTON_RIGHT || pressedButton == BUTTON_UP || pressedButton == BUTTON_DOWN)) {
            pacManNextDirection = pressedButton;

            if (pacManDirection != pacManNextDirection) {
              if (pacManNextDirection == DIRECTION_RIGHT) {
                if (isPacManStoped) {
                  Tile tile = pacManTile;

                  if (tile.column < numberOfColumns - 1) {
                    tile.column++;
                  }

                  if (!isWallTile(tile)) {
                    pacManDirection = pacManNextDirection;
                    isPacManStoped = false;
                  } else {
                    pacManNextDirection = pacManDirection;
                  }
                }
              } else if (pacManNextDirection == DIRECTION_LEFT) {
                if (isPacManStoped) {
                  Tile tile = pacManTile;

                  if (tile.column > 0) {
                    tile.column--;
                  }

                  if (!isWallTile(tile)) {
                    pacManDirection = pacManNextDirection;
                    isPacManStoped = false;
                  } else {
                    pacManNextDirection = pacManDirection;
                  }
                }
              } else if (pacManNextDirection == DIRECTION_DOWN) {
                if (isPacManStoped) {
                  Tile tile = pacManTile;

                  if (tile.row < numberOfRows - 1) {
                    tile.row++;
                  }

                  if (!isWallTile(tile)) {
                    pacManDirection = pacManNextDirection;
                    isPacManStoped = false;
                  } else {
                    pacManNextDirection = pacManDirection;
                  }
                }
              } else if (pacManNextDirection == DIRECTION_UP) {
                if (isPacManStoped) {
                  Tile tile = pacManTile;

                  if (tile.row > 0) {
                    tile.row--;
                  }

                  if (!isWallTile(tile)) {
                    pacManDirection = pacManNextDirection;
                    isPacManStoped = false;
                  } else {
                    pacManNextDirection = pacManDirection;
                  }
                }
              }
            }
          }
        } else {
          if (isSoundEnabled && ((!isGamepadEnabled && i == BUTTON_RIGHT) || (isGamepadEnabled && isVolumeChanged > 0.0f))) {
            if (!isGamepadEnabled && i == BUTTON_RIGHT) {
              toggleVolume();
            }

            if ((!isGamepadEnabled && i == BUTTON_RIGHT) || (isGamepadEnabled && isVolumeChanged > 0.0f && (i == BUTTON_UP || i == BUTTON_DOWN))) {
              titleTime = 250.0f;
              playSound(SOUND_VOLUME);
            }
          } else if (i == pressedButton && (pressedButton == BUTTON_A || pressedButton == BUTTON_B)) {
            if (isAutoPlay) {
              if (gameMode != GAME_MODE_TITLE) {
                readHighScore();
                resetGame();
                newLevel();
                changeGameMode(GAME_MODE_TITLE);
              }
            } else {
              if (gameMode == GAME_MODE_TITLE) {
                if (titleTime >= 5250.0f) {
                  readHighScore();
                  changeGameMode(GAME_MODE_TITLE);
                } else {
                  newLevel();
                  changeGameMode(GAME_MODE_READY);
                }
              } else if (gameMode == GAME_MODE_GAME_OVER) {
                changeGameMode(GAME_MODE_TITLE);
              }
            }
          } else if (isMenuEnabled && gameMode == GAME_MODE_TITLE && titleTime >= 250.0f && titleTime < 5250.0f && pressedButton == BUTTON_LEFT && i == BUTTON_LEFT) {
            isRequestingExit = true;
          }

          pressedButton = BUTTON_NONE;
        }
        break;
      }
    }
  }
}

bool PacMan::readGhostPath(int i) {
  bool found = false;

  if (ghostNumberOfPaths[i] > 0 && ++ghostCurrentPath[i] < ghostNumberOfPaths[i]) {
    int direction = getDirection(ghostTile[i], ghostPath[i][ghostCurrentPath[i]]);

    if (direction != DIRECTION_NONE) {
      ghostDirection[i] = direction;
      ghostNextDirection[i] = ghostDirection[i];
      found = true;
    }
  }

  if (!found) {
    resetGhostPath(i);

    if (ghostState[i] == GHOST_STATE_DIE) {
      if (ghostTile[i].row == 13 && ghostTile[i].column == 10) {
        spawnGhost(i, {13, 10}, GHOST_STATE_GO, DIRECTION_DOWN);
      } else {
        ghostDie(i);
      }
    }
  }

  ghostRandomTime[i] = maxGhostRandomTime;

  return found;
}

void PacMan::render() {
  if (!isMenuEnabled) {
    espert->oled.clear(false);
  }

  switch (gameMode) {
    case GAME_MODE_TITLE:
      renderTitle();
      break;

    case GAME_MODE_READY:
    case GAME_MODE_PLAY:
    case GAME_MODE_DIE:
    case GAME_MODE_SUCCESS:
    case GAME_MODE_GAME_OVER:
      renderWorld();

      if (gameMode == GAME_MODE_READY) {
        drawBitmap(worldPosition.x + readyPosition.x, worldPosition.y + readyPosition.y, 40, 8, readyBitmap, readyMaskBitmap);
      } else if (gameMode == GAME_MODE_GAME_OVER) {
        drawBitmap(worldPosition.x + gameOverPosition.x, worldPosition.y + gameOverPosition.y, 64, 8, gameOverBitmap, gameOverMaskBitmap);
      }

      renderItem();
      renderEdibleGhost();
      renderPacMan();
      renderGhost();
      renderHUD();
      renderFPS(67, 58, 8, 8, -4, (const uint8_t*)numberBitmap, (const uint8_t*)numberMaskBitmap, ESPERT_WHITE, 53, 58);
      break;
  }

  if (!isMenuEnabled) {
    espert->oled.update();
  }
}

void PacMan::renderEdibleGhost() {
  if (gameMode == GAME_MODE_GAME_OVER) {
  } else if (gameMode == GAME_MODE_DIE && pacManDieTime > 2000.0f) {
  } else if (gameMode == GAME_MODE_SUCCESS && successTime > 2000.0f) {
  } else {
    for (int i = 0; i < numberOfGhosts; i++) {
      if (isGhostVisibled[i]) {
        if (ghostState[i] == GHOST_STATE_DIE) {
          if (i != ghostEdiblePointGhost) {
            drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostEyeBitmap[ghostDirection[i]], ghostEyeMaskBitmap[ghostDirection[i]], ESPERT_BLACK);
          }
        } else if (isGhostEdible[i]) {
          if (powerPelletTime > minPowerPelletTime || (powerPelletTime <= minPowerPelletTime && isBlink(0.5f))) {
            drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostEdibleBitmap[(int)ghostFrame[i]], ghostMaskBitmap[(int)ghostFrame[i]]);
          } else if (powerPelletTime <= minPowerPelletTime) {
            drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostBitmap[(int)ghostFrame[i]], ghostMaskBitmap[(int)ghostFrame[i]]);
            drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostEyeBitmap[ghostDirection[i]], NULL, ESPERT_BLACK);
          }
        }
      }
    }
  }
}

void PacMan::renderGhost() {
  if (gameMode == GAME_MODE_GAME_OVER) {
  } else if (gameMode == GAME_MODE_DIE && pacManDieTime > 2000.0f) {
  } else if (gameMode == GAME_MODE_SUCCESS && successTime > 2000.0f) {
  } else {
    for (int i = 0; i < numberOfGhosts; i++) {
      if (isGhostVisibled[i] && !isGhostEdible[i] && ghostState[i] != GHOST_STATE_DIE) {
        drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostBitmap[(int)ghostFrame[i]], ghostMaskBitmap[(int)ghostFrame[i]]);
        drawBitmap(worldPosition.x + ghostPosition[i].x, worldPosition.y + ghostPosition[i].y, 16, 16, ghostEyeBitmap[ghostDirection[i]], NULL, ESPERT_BLACK);
      }
    }
  }
}

void PacMan::renderHUD() {
  drawBitmap(7, -1, 32, 8, highScoreBitmap, highScoreMaskBitmap);
  String highScoreString = String(highScore);
  int n = 0;
  for (int i = 0; i < highScoreString.length(); i++) {
    n = highScoreString.charAt(i) - '0';
    drawBitmap(37 + (i * (numberSize.width - 1)), -1, 8, 8, numberBitmap[n], numberMaskBitmap[n]);
  }

  String scoreString = String(score);
  for (int i = 0; i < scoreString.length(); i++) {
    n = scoreString.charAt(scoreString.length() - i - 1) - '0';
    drawBitmap(screenSize.width - 8 - ((i + 1) * (numberSize.width - 1)), -1, 8, 8, numberBitmap[n], numberMaskBitmap[n]);
  }

  for (int i = 0; i < numberOfPacManLives; i++) {
    drawBitmap(7 + (i * 6), 58, 8, 8, lifeBitmap, lifeMaskBitmap);
  }

  for (int i = 0; i < 4 && level - i >= 0; i++) {
    int n = level - i;

    if (n > maxItems - 1) {
      n = maxItems - 1;
    }

    drawBitmap(screenSize.width - 8 - ((i + 1) * itemSize.width), screenSize.height - itemSize.height + 1, 16, 16, itemBitmap[item[n]], itemMaskBitmap[item[n]]);
  }
}

void PacMan::renderItem() {
  if (gameMode != GAME_MODE_READY && gameMode != GAME_MODE_GAME_OVER) {
    if (itemVisibledTime > 0.0f) {
      drawBitmap(worldPosition.x + itemPosition.x, worldPosition.y + itemPosition.y, 16, 16, itemBitmap[itemIndex], itemMaskBitmap[itemIndex]);
    } else if (itemVisibledTime < 0.0f) {
      int n = 0;
      for (int i = 0; i < itemPointString.length(); i++) {
        n = itemPointString.charAt(i) - '0';
        drawBitmap(worldPosition.x + itemPointPosition.x + (i * (numberSize.width - 1)), worldPosition.y + itemPointPosition.y, 8, 8, numberBitmap[n], numberMaskBitmap[n]);
      }
    }
  }
}

void PacMan::renderPacMan() {
  if (gameMode == GAME_MODE_GAME_OVER) {
  } else if (gameMode == GAME_MODE_DIE && pacManDieTime > 2000.0f) {
    if (pacManDieFrame != -1.0f) {
      drawBitmap(worldPosition.x + pacManPosition.x + 1, worldPosition.y + pacManPosition.y + 1, 16, 16, dieBitmap[(int)pacManDieFrame]);
    }
  } else if (gameMode == GAME_MODE_SUCCESS && successTime > 2000.0f) {
    drawBitmap(worldPosition.x + pacManPosition.x, worldPosition.y + pacManPosition.y, 16, 16, pacManBitmap[pacManDirection][(int)pacManFrame], pacManMaskBitmap[pacManDirection][(int)pacManFrame]);
  } else {
    if (isPacManVisibled) {
      if (ghostEdiblePointVisibledTime > 0.0f) {
        int n = 0;
        for (int i = 0; i < ghostEdiblePointString.length(); i++) {
          n = ghostEdiblePointString.charAt(i) - '0';
          drawBitmap(worldPosition.x + ghostEdiblePointPosition.x + (i * (numberSize.width - 1)), worldPosition.y + ghostEdiblePointPosition.y, 8, 8, numberBitmap[n], numberMaskBitmap[n]);
        }
      } else {
        drawBitmap(worldPosition.x + pacManPosition.x, worldPosition.y + pacManPosition.y, 16, 16, pacManBitmap[pacManDirection][(int)pacManFrame], pacManMaskBitmap[pacManDirection][(int)pacManFrame]);
      }
    }
  }
}

void PacMan::renderTitle() {
  if (titleTime >= 250.0f && titleTime < 5250.0f) {
    drawBitmap(0, 11, 128, 32, titleBitmap);
    drawBitmap(49, 41, 32, 8, highScoreBitmap);
    String highScoreString = String(highScore);
    int x = (screenSize.width - (highScoreString.length() * (numberSize.width - 1))) * 0.5f;
    int n = 0;
    for (int i = 0; i < highScoreString.length(); i++) {
      n = highScoreString.charAt(i) - '0';
      drawBitmap(x + (i * (numberSize.width - 1)), 48, 8, 8, numberBitmap[n], numberMaskBitmap[n]);
    }

    renderVolume(0, 0);
    renderBattery(screenSize.width - batterySize.width, 0);
    renderMakerAsia(40, 54);
  }

  if (!isAutoPlay && titleTime >= 5250.0f) {
    float t = 5500.0f;

    if (titleTime > t) {
      drawBitmap(17, 0, 104, 8, characterNicknameBitmap);
    }

    for (int i = 0; i < numberOfGhosts; i++) {
      t += 880.0f;
      if (titleTime > t) {
        drawBitmap(26, 11 + (i * 10), 40, 8, characterBitmap[i]);
      }

      t += 50.0f;
      if (titleTime > t) {
        int y = 6 + (i * (ghostSize.height - 1));
        drawBitmap(7, y, 16, 16, ghostBitmap[0]);
        drawBitmap(7, y, 16, 16, ghostEyeBitmap[DIRECTION_RIGHT], NULL, ESPERT_BLACK);
      }

      t += 830.0f;
      if (titleTime > t) {
        drawBitmap(81, 11 + (i * 10), 40, 8, nicknameBitmap[i]);
      }
    }

    t += 880.0f;
    if (titleTime > t && titleTime <= t + 2000.0f) {
      drawBitmap(45, 51, 40, 16, ptsBitmap);
    }

    t += 2000.0f; // 15420
    if (titleTime > t) {
      if (pacManDirection == DIRECTION_LEFT && isBlink(0.5f)) {
        drawBitmap(17, 56, 8, 8, tileBitmap[TILE_TYPE_POWER_PELLET]);
      }

      for (int i = ghostEdiblePointIndex; i < numberOfGhosts; i++) {
        if (pacManDirection == DIRECTION_LEFT) {
          drawBitmap(ghostPosition[i].x, ghostPosition[i].y, 16, 16, ghostBitmap[(int)ghostFrame[i]], ghostMaskBitmap[(int)ghostFrame[i]]);
          drawBitmap(ghostPosition[i].x, ghostPosition[i].y, 16, 16, ghostEyeBitmap[ghostDirection[i]], NULL, ESPERT_BLACK);
        } else {
          drawBitmap(ghostPosition[i].x, ghostPosition[i].y, 16, 16, ghostEdibleBitmap[(int)ghostFrame[i]], ghostMaskBitmap[(int)ghostFrame[i]]);
        }
      }

      if (ghostEdiblePointVisibledTime > 0.0f) {
        int n = 0;
        for (int i = 0; i < ghostEdiblePointString.length(); i++) {
          n = ghostEdiblePointString.charAt(i) - '0';
          drawBitmap(ghostEdiblePointPosition.x + (i * (numberSize.width - 1)), ghostEdiblePointPosition.y, 8, 8, numberBitmap[n], numberMaskBitmap[n]);
        }
      } else {
        drawBitmap(pacManPosition.x, pacManPosition.y, 16, 16, pacManBitmap[pacManDirection][(int)pacManFrame], pacManMaskBitmap[pacManDirection][(int)pacManFrame]);
      }
    }
  }

  renderFPS(120, 57, 8, 8, -4, (const uint8_t*)numberBitmap, (const uint8_t*)numberMaskBitmap, ESPERT_WHITE, 106, 57);
}

void PacMan::renderWorld() {
  int y = worldPosition.y;

  for (byte row = 0; row < numberOfRows; row++) {
    if (y > -tileSize.height && y < screenSize.height + tileSize.height) {
      int x = worldPosition.x;

      for (byte column = 0; column < numberOfColumns; column++) {
        int tileType = world[row][column];

        if (tileType != TILE_TYPE_SPACE && (tileType != TILE_TYPE_POWER_PELLET || (tileType == TILE_TYPE_POWER_PELLET && isBlink()))) {
          if (gameMode != GAME_MODE_SUCCESS || (gameMode == GAME_MODE_SUCCESS && successTime <= 2000.0f) || (gameMode == GAME_MODE_SUCCESS && successTime > 2000.0f && isBlink(0.5f))) {
            drawBitmap(x, y, 8, 8, tileBitmap[tileType]);
            ESP.wdtFeed();
          }
        }

        x += tileSize.width;
      }
    }

    y += tileSize.height;
  }
}

void PacMan::resetGame() {
  isAutoPlay = false;
  level = 0;
  score = 0l;
  isAwardExtraLife = false;
  numberOfPacManLives = startPacManLives;
  itemTile = {15, 10};
  itemPosition = getPositionFromTile(itemTile, itemSize);
  restartLevel();
}

void PacMan::resetGhostPath(int i) {
  memset(&ghostPath[i], 0, sizeof(ghostPath[i]));
  ghostNumberOfPaths[i] = 0;
  ghostCurrentPath[i] = -1;
}

void PacMan::resetPacManIncreseSpeed() {
  pacManIncreseSpeed = 0.0f;
  pacManIncreseSpeedTime = pacManMaxIncreseSpeedTime;
}

void PacMan::restartLevel() {
  successTime = 0.0f;

  readyTime = 0.0f;
  Tile tile = {15, 10};
  Size readySize = {40, 8};
  readyPosition = getPositionFromTile(tile, readySize);

  gameOverTime = 0.0f;
  Size gameOverSize = {59, 8};
  gameOverPosition = getPositionFromTile(tile, gameOverSize);

  // pac-man
  pacManTile = {20, 10};
  pacManLastTile = pacManTile;
  pacManPosition = getPositionFromTile(pacManTile, pacManSize);
  worldPosition = {(screenSize.width - worldPixelSize.width) * 0.5f, screenSize.height - worldPixelSize.height};
  scrollWorldToPacManPosition();
  worldStartPosition = worldPosition.y;
  worldPosition.y = 0.0f;
  pacManDirection = DIRECTION_LEFT;
  pacManNextDirection = pacManDirection + (isAutoPlay ? random(0, 2) : 0);
  pacManFrame = 0.0f;
  isPacManStoped = false;
  isPacManVisibled = false;
  powerPelletTime = 0.0f;
  blinkTime = 0.0f;
  pacManDieTime = 0.0f;
  pacManDieFrame = 0.0f;
  isPacManDisappeared = false;
  resetPacManIncreseSpeed();

  // ghost
  ghostEdiblePointIndex = 0;
  ghostEdiblePointVisibledTime = 0.0f;
  ghostEdiblePointGhost = -1;
  for (int i = 0; i < numberOfGhosts; i++) {
    if (i == 0) {
      spawnGhost(i, {13, 11}, GHOST_STATE_WAIT, DIRECTION_UP, (level == 0) ? 36000.0f : (level == 0) ? 16000.0f : 0.0f);
    } else if (i == 1) {
      spawnGhost(i, {13, 9}, GHOST_STATE_WAIT, DIRECTION_UP, (level == 0) ? 6000.0f : 0.0f);
    } else if (i == 2) {
      spawnGhost(i, {13, 10}, GHOST_STATE_GO, DIRECTION_UP);
    } else if (i == 3) {
      spawnGhost(i, {10, 10}, GHOST_STATE_RUN, DIRECTION_LEFT + random(0, 2));
    }

    ghostLastTile[i] = ghostTile[i];
    ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
    ghostFrame[i] = 0.0f;
    isGhostStoped[i] = false;
    isGhostVisibled[i] = false;
    isGhostEdible[i] = false;
    ghostRandomTime[i] = maxGhostRandomTime;
    isRandomGhostPath[i] = false;
    isRunaway[i] = false;
    resetGhostPath(i);
  }
}

bool PacMan::randomGhostPath(int i) {
  if (isRandomGhostPath[i]) {
    isRandomGhostPath[i] = false;
    ghostTile[i] = ghostLastTile[i];
    ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);

    // find pac-man area
    int pacManCorner = 0;
    if (isRunaway[i]) {
      if (pacManTile.row <= numberOfRows * 0.5f) { // upper
        if (pacManTile.column <= numberOfColumns * 0.5f) { // left
          pacManCorner = 0;
        } else { // right
          pacManCorner = 1;
        }
      } else { // lower
        if (pacManTile.column <= numberOfColumns * 0.5f) { // left
          pacManCorner = 2;
        } else { // right
          pacManCorner = 3;
        }
      }
    }

    int availabledCorners[3] = {0};
    int n = 0;
    for (int corner = 0; corner < 4; corner++) {
      if (!isRunaway[i] || (isRunaway[i] && corner != pacManCorner)) {
        availabledCorners[n++] = corner;
      }
    }

    isRunaway[i] = false;
    int ghostCorner = random(0, 3); // random 0 to 2
    if (pathFinding(i, cornerTile[availabledCorners[ghostCorner]])) {
      readGhostPath(i);
    }

    return true;
  }

  return false;
}

void PacMan::scrollWorldToPacManPosition() {
  if (gameMode != GAME_MODE_GAME_OVER) {
    float y = round(pacManPosition.y + (pacManSize.height * 0.5f));
    if (y > screenCenter.y - statusBarSize.height - 1.0f && y < worldPixelSize.height - screenCenter.y) {
      worldPosition.y = screenCenter.y - y;
    }
  }
}

void PacMan::scrollWorldToPosition(float y) {
  if (worldPosition.y > y) {
    worldPosition.y -= elapsedTime * 22.0f / 1000.0f;

    if (worldPosition.y <= y) {
      worldPosition.y = y;
    }
  } else if (worldPosition.y < y) {
    worldPosition.y += elapsedTime * 22.0f / 1000.0f;

    if (worldPosition.y >= y) {
      worldPosition.y = y;
    }
  }
}

void PacMan::setRandomGhostPath(int i, bool runaway) {
  if (ghostState[i] == GHOST_STATE_RUN) {
    resetGhostPath(i);
    ghostRandomTime[i] = 0.0f;
    isRandomGhostPath[i] = true;
    isRunaway[i] = runaway;
  }
}

void PacMan::spawnGhost(int i, Tile tile, int state, int direction, int waitTime) {
  ghostTile[i] = tile;
  ghostState[i] = state;
  ghostDirection[i] = direction;
  ghostNextDirection[i] = direction;
  ghostWaitTime[i] = waitTime;
}

void PacMan::update() {
  updateGameTime();

  switch (gameMode) {
    case GAME_MODE_TITLE:
      updateTitle();
      break;

    case GAME_MODE_READY:
      if (readyTime >= 1000.0f) {
        scrollWorldToPosition(worldStartPosition);
      }

      if (readyTime < maxReadyTime) {
        readyTime += elapsedTime;

        if (readyTime >= 3000.0f && !isPacManVisibled) {
          numberOfPacManLives--;
          isPacManVisibled = true;

          for (int i = 0; i < numberOfGhosts; i++) {
            isGhostVisibled[i] = true;
          }
        }
      } else {
        changeGameMode(GAME_MODE_PLAY);
      }
      break;

    case GAME_MODE_DIE:
    case GAME_MODE_PLAY:
    case GAME_MODE_SUCCESS:
    case GAME_MODE_GAME_OVER:
      if (gameMode == GAME_MODE_DIE) {
        pacManDieTime += elapsedTime;

        if (pacManDieTime > 2000.0f) {
          if (pacManDieFrame != -1.0f) {
            pacManDieFrame += elapsedTime * 10.0f / 1000.0f;

            if (!isPacManDisappeared && pacManDieFrame >= numberOfDieFrames - 2) {
              playSound(SOUND_DISAPPEAR);
              isPacManDisappeared = true;
            } else if (pacManDieFrame >= numberOfDieFrames) {
              pacManDieFrame = -1.0f;
            }
          }

          if (pacManDieFrame == -1.0f && pacManDieTime > 5000.0f) {
            if (isAutoPlay) {
              exitAutoPlay();
            } else {
              if (numberOfPacManLives == 0) {
                changeGameMode(GAME_MODE_GAME_OVER);
              } else {
                restartLevel();
                changeGameMode(GAME_MODE_READY);
              }
            }
            break;
          }
        }
      } else if (gameMode == GAME_MODE_SUCCESS) {
        successTime += elapsedTime;

        if (successTime > 2000.0f) {
          pacManFrame = numberOfPacManFrames - 1;

          if (successTime > 5000.0f) {
            if (isAutoPlay) {
              exitAutoPlay();
            } else {
              if (++level >= numberOfLevels) {
                level = 0;
              }

              numberOfPacManLives++;
              newLevel();
              changeGameMode(GAME_MODE_READY);
            }
            break;
          }
        }
      } else if (gameMode == GAME_MODE_GAME_OVER) {
        scrollWorldToPosition(worldStartPosition + (tileSize.height * 5.0f));
        gameOverTime += elapsedTime;

        if (gameOverTime > 5000.0f) {
          changeGameMode(GAME_MODE_TITLE);
          break;
        }
      }

      if (gameMode == GAME_MODE_PLAY) {
        if (itemVisibledTime > 0.0f) {
          itemVisibledTime -= elapsedTime;
          if (itemVisibledTime <= 0.0f) {
            itemVisibledTime = 0.0f;
          }
        } else if (itemVisibledTime < 0.0f) {
          itemVisibledTime += elapsedTime;
          if (itemVisibledTime >= 0.0f) {
            itemVisibledTime = 0.0f;
          }
        }
      }

      updateGhost();
      updatePacMan();
      break;
  }

  ESP.wdtFeed();
}

void PacMan::updateGhost() {
  if (ghostEdiblePointVisibledTime > 0.0f) {
    ghostEdiblePointVisibledTime -= elapsedTime;

    if (ghostEdiblePointVisibledTime <= 0.0f) {
      ghostEdiblePointVisibledTime = 0.0f;
      ghostEdiblePointGhost = -1;
    }
  }

  for (int i = 0; i < numberOfGhosts; i++) {
    if (!isGhostStoped[i]) {
      ghostFrame[i] += elapsedTime * ghostAnimationSpeed;

      if (ghostFrame[i] >= numberOfGhostFrames) {
        ghostFrame[i] = 0.0f;
      }

      if (ghostWaitTime[i] > 0.0f) {
        ghostWaitTime[i] -= elapsedTime;

        if (ghostWaitTime[i] <= 0.0f) {
          ghostWaitTime[i] = 0.0f;
        }
      }

      bool isHitWall = false;
      bool isDoorTileEnabled = (ghostState[i] == GHOST_STATE_GO) ? false : true;
      float ghostSpeed = gameSpeed;

      if (ghostState[i] == GHOST_STATE_DIE) {
        ghostSpeed = gameNormalSpeed * 3.0f;
      } else if (ghostState[i] == GHOST_STATE_WAIT || ghostState[i] == GHOST_STATE_GO) {
        ghostSpeed = gameNormalSpeed * 0.6f;
      } else if (isGhostEdible[i] || (ghostTile[i].row == 13 && (ghostTile[i].column <= 3 || ghostTile[i].column >= numberOfColumns - 4))) { // tunnel
        ghostSpeed *= 0.5f;
      }

      if (gameMode == GAME_MODE_GAME_OVER || gameMode == GAME_MODE_SUCCESS || gameMode == GAME_MODE_DIE || (ghostEdiblePointVisibledTime > 0.0f && (ghostState[i] != GHOST_STATE_DIE || (ghostState[i] == GHOST_STATE_DIE && i == ghostEdiblePointGhost)))) {
        ghostSpeed = 0.0f;
      }

      if (ghostDirection[i] == DIRECTION_LEFT) {
        ghostPosition[i].x -= elapsedTime * ghostSpeed;
        ghostTile[i] = getTileFromPosition(ghostPosition[i], ghostSize);

        if (!isGhostPath(i)) {
          isHitWall = isWallTile(ghostTile[i], isDoorTileEnabled);

          if (isHitWall && ghostTile[i].column < numberOfColumns - 1) {
            ghostTile[i].column++;
          } else if (ghostTile[i].column == -2) {
            ghostTile[i].column = numberOfColumns;
            ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
          }
        }
      } else if (ghostDirection[i] == DIRECTION_RIGHT) {
        ghostPosition[i].x += elapsedTime * ghostSpeed;
        Point pos = ghostPosition[i];
        pos.x += tileSize.width;
        ghostTile[i] = getTileFromPosition(pos, ghostSize);

        if (!isGhostPath(i)) {
          isHitWall = isWallTile(ghostTile[i], isDoorTileEnabled);

          if (isHitWall && ghostTile[i].column > 0) {
            ghostTile[i].column--;
          } else if (ghostTile[i].column == numberOfColumns + 1) {
            ghostTile[i].column = -1;
            ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
          }
        }
      } else if (ghostDirection[i] == DIRECTION_UP) {
        ghostPosition[i].y -= elapsedTime * ghostSpeed;
        ghostTile[i] = getTileFromPosition(ghostPosition[i], ghostSize);

        if (!isGhostPath(i)) {
          isHitWall = isWallTile(ghostTile[i], isDoorTileEnabled);

          if (isHitWall && ghostTile[i].row < numberOfRows - 1) {
            ghostTile[i].row++;
          }
        }
      } else if (ghostDirection[i] == DIRECTION_DOWN) {
        ghostPosition[i].y += elapsedTime * ghostSpeed;
        Point pos = ghostPosition[i];
        pos.y += tileSize.height;
        ghostTile[i] = getTileFromPosition(pos, ghostSize);

        if (!isGhostPath(i)) {
          isHitWall = isWallTile(ghostTile[i], isDoorTileEnabled);

          if (isHitWall && ghostTile[i].row > 0) {
            ghostTile[i].row--;
          }
        }
      }

      if (isHitWall) {
        ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);

        if (ghostState[i] == GHOST_STATE_WAIT) {
          if (ghostDirection[i] == DIRECTION_DOWN) {
            if (ghostWaitTime[i] == 0.0f) {
              ghostState[i] = GHOST_STATE_GO;
              resetGhostPath(i);
              addGhostPath(i, {13, 10});
              addGhostPath(i, {12, 10});
              addGhostPath(i, {11, 10});
              addGhostPath(i, {10, 10});
              readGhostPath(i);
            } else {
              ghostDirection[i] = DIRECTION_UP;
              ghostNextDirection[i] = ghostDirection[i];
            }
          } else {
            ghostDirection[i] = DIRECTION_DOWN;
            ghostNextDirection[i] = ghostDirection[i];
          }
        } else if (ghostState[i] == GHOST_STATE_GO) {
          if (ghostDirection[i] == DIRECTION_UP) {
            ghostState[i] = GHOST_STATE_RUN;
            ghostDirection[i] = DIRECTION_LEFT + random(0, 2); // random 0 to 1
            ghostNextDirection[i] = ghostDirection[i];
            ghostRandomTime[i] = maxGhostRandomTime;
            setRandomGhostPath(i, false);
          } else {
            ghostDirection[i] = DIRECTION_UP;
            ghostNextDirection[i] = ghostDirection[i];
          }
        } else if (ghostState[i] == GHOST_STATE_RUN) {
          if (ghostNextDirection[i] == ghostDirection[i]) {
            ghostRandomTime[i] = maxGhostRandomTime;
            int n = random(0, 3); // random 0 to 2

            if (n < 2) { // force cross direction
              if (ghostDirection[i] == DIRECTION_LEFT || ghostDirection[i] == DIRECTION_RIGHT) {
                ghostNextDirection[i] = DIRECTION_UP + n;

                if (!isTileAvailabled(ghostNextDirection[i], ghostTile[i])) {
                  ghostNextDirection[i] = DIRECTION_UP + (1 - n);
                }
              } else {
                ghostNextDirection[i] = DIRECTION_LEFT + n;

                if (!isTileAvailabled(ghostNextDirection[i], ghostTile[i])) {
                  ghostNextDirection[i] = DIRECTION_LEFT + (1 - n);
                }
              }
            } else {
              chasePacMan(i);
            }
          }

          ghostDirection[i] = ghostNextDirection[i];
        }
      } else if (ghostNumberOfPaths[i] == 0) {
        if (ghostState[i] == GHOST_STATE_RUN) {
          ghostRandomTime[i] -= elapsedTime;

          if (ghostRandomTime[i] <= 0.0f) {
            ghostRandomTime[i] = 0.0f;

            // change direction while running
            if (ghostLastTile[i].row >= 0 && ghostLastTile[i].row < numberOfRows && ghostLastTile[i].column >= 0 && ghostLastTile[i].column < numberOfColumns) {
              if (isJustMoveToNewTile(ghostTile[i], ghostLastTile[i])) {
                if (!randomGhostPath(i) && random(0, 2) == 0) { // random 0 to 1
                  ghostRandomTime[i] = maxGhostRandomTime;
                  int n = random(0, 3); // random 0 to 2

                  if (n < 2) { // cross direction, if tile availabled
                    if (ghostDirection[i] == DIRECTION_LEFT || ghostDirection[i] == DIRECTION_RIGHT) {
                      ghostNextDirection[i] = DIRECTION_UP + n;
                    } else {
                      ghostNextDirection[i] = DIRECTION_LEFT + n;
                    }

                    if (isTileAvailabled(ghostNextDirection[i], ghostLastTile[i]), isDoorTileEnabled) {
                      ghostTile[i] = ghostLastTile[i];
                      ghostPosition[i] = getPositionFromTile(ghostTile[i], ghostSize);
                      ghostDirection[i] = ghostNextDirection[i];
                    } else {
                      ghostNextDirection[i] = ghostDirection[i];
                    }
                  } else {
                    chasePacMan(i);
                  }
                }
              }
            }
          }
        }
      }

      ghostLastTile[i] = ghostTile[i];
    }
  }
}

void PacMan::updatePacMan() {
  if (powerPelletTime > 0.0f && ghostEdiblePointVisibledTime == 0.0f) {
    powerPelletTime -= elapsedTime;

    if (powerPelletTime <= 0.0f) {
      powerPelletTime = 0.0f;
      memset(&isGhostEdible, false, sizeof(isGhostEdible));
    }
  }

  if (!isPacManStoped) {
    if (gameMode == GAME_MODE_PLAY) {
      if (pacManIncreseSpeedTime > 0.0f) {
        pacManIncreseSpeedTime -= elapsedTime;

        if (pacManIncreseSpeedTime <= 0.0f) {
          pacManIncreseSpeedTime = 0.0f;
        }
      } else if (pacManIncreseSpeed < pacManMaxIncreseSpeed) {
        pacManIncreseSpeed += elapsedTime * 0.002f;

        if (pacManIncreseSpeed >= pacManMaxIncreseSpeed) {
          pacManIncreseSpeed = pacManMaxIncreseSpeed;
        }
      }
    }

    float pacManSpeed = gameSpeed + (pacManIncreseSpeed / 1000.0f);
    if (gameMode == GAME_MODE_GAME_OVER || gameMode == GAME_MODE_SUCCESS || gameMode == GAME_MODE_DIE || ghostEdiblePointVisibledTime > 0.0f) {
      pacManSpeed = 0.0f;
    }

    if (pacManSpeed > 0.0f) {
      pacManFrame += elapsedTime * pacManAnimationSpeed;

      if (pacManFrame >= numberOfPacManFrames) {
        pacManFrame = 0.0f;
      }
    }

    bool isHitWall = false;

    if (pacManDirection == DIRECTION_LEFT) {
      pacManPosition.x -= elapsedTime * pacManSpeed;
      pacManTile = getTileFromPosition(pacManPosition, pacManSize);
      isHitWall = isWallTile(pacManTile);

      if (isHitWall && pacManTile.column < numberOfColumns - 1) {
        pacManTile.column++;
      } else if (pacManTile.column == -2) {
        pacManTile.column = numberOfColumns;
        pacManPosition = getPositionFromTile(pacManTile, pacManSize);
      }
    } else if (pacManDirection == DIRECTION_RIGHT) {
      pacManPosition.x += elapsedTime * pacManSpeed;
      Point pos = pacManPosition;
      pos.x += tileSize.width;
      pacManTile = getTileFromPosition(pos, pacManSize);
      isHitWall = isWallTile(pacManTile);

      if (isHitWall && pacManTile.column > 0) {
        pacManTile.column--;
      } else if (pacManTile.column == numberOfColumns + 1) {
        pacManTile.column = -1;
        pacManPosition = getPositionFromTile(pacManTile, pacManSize);
      }
    } else if (pacManDirection == DIRECTION_UP) {
      pacManPosition.y -= elapsedTime * pacManSpeed;
      pacManTile = getTileFromPosition(pacManPosition, pacManSize);
      isHitWall = isWallTile(pacManTile);

      if (isHitWall && pacManTile.row < numberOfRows - 1) {
        pacManTile.row++;
      }
    } else if (pacManDirection == DIRECTION_DOWN) {
      pacManPosition.y += elapsedTime * pacManSpeed;
      Point pos = pacManPosition;
      pos.y += tileSize.height;
      pacManTile = getTileFromPosition(pos, pacManSize);
      isHitWall = isWallTile(pacManTile);

      if (isHitWall && pacManTile.row > 0) {
        pacManTile.row--;
      }
    }

    if (isHitWall) {
      pacManPosition = getPositionFromTile(pacManTile, pacManSize);

      if (pacManDirection != pacManNextDirection) {
        if (isTileAvailabled(pacManNextDirection, pacManTile)) {
          pacManDirection = pacManNextDirection;
        } else {
          pacManNextDirection = pacManDirection;
        }
      } else {
        isPacManStoped = true;
        resetPacManIncreseSpeed();

        if ((int)pacManFrame == numberOfPacManFrames - 1) {
          pacManFrame = 0.0f;
        }
      }
    } else if (pacManDirection != pacManNextDirection) {
      if (isJustMoveToNewTile(pacManTile, pacManLastTile)) {
        if (isTileAvailabled(pacManNextDirection, pacManLastTile)) {
          if (((pacManDirection == DIRECTION_LEFT || pacManDirection == DIRECTION_RIGHT) && (pacManNextDirection == DIRECTION_UP || pacManNextDirection == DIRECTION_DOWN)) ||
              ((pacManDirection == DIRECTION_UP || pacManDirection == DIRECTION_DOWN) && (pacManNextDirection == DIRECTION_LEFT || pacManNextDirection == DIRECTION_RIGHT))) {
            pacManTile = pacManLastTile;
            pacManPosition = getPositionFromTile(pacManTile, pacManSize);
          }

          pacManDirection = pacManNextDirection;
        }
      }
    }

    if (isAutoPlay) { // change direction
      int n = random(0, (isHitWall && isPacManStoped) ? 2 : 100);

      if (n < 2) {
        if (pacManDirection == DIRECTION_LEFT || pacManDirection == DIRECTION_RIGHT) {
          pacManNextDirection = DIRECTION_UP + n;

          if (!isTileAvailabled(pacManNextDirection, pacManTile)) {
            pacManNextDirection = DIRECTION_UP + (1 - n);
          }
        } else {
          pacManNextDirection = DIRECTION_LEFT + n;

          if (!isTileAvailabled(pacManNextDirection, pacManTile)) {
            pacManNextDirection = DIRECTION_LEFT + (1 - n);
          }
        }

        if (isHitWall && isPacManStoped) {
          pacManDirection = pacManNextDirection;
          isPacManStoped = false;
        }
      }
    }

    pacManLastTile = pacManTile;
    scrollWorldToPacManPosition();
  }

  eat(pacManTile);
}

void PacMan::updateTitle() {
  bool finished = false;
  float speed = gameNormalSpeed * 1.5f;
  titleTime += ((isVolumeChanged > 0.0f) ? 0.0f : elapsedTime);

  if (isAutoPlay && titleTime > 24547.0f) {
    newLevel();
    changeGameMode(GAME_MODE_READY);
  } else if (titleTime > 15420.0f) {
    if (ghostEdiblePointVisibledTime == 0.0f) {
      pacManFrame += elapsedTime * pacManAnimationSpeed;

      if (pacManFrame >= numberOfPacManFrames) {
        pacManFrame = 0.0f;
      }

      pacManPosition.x += elapsedTime * speed * (pacManDirection == DIRECTION_LEFT ? -1.0f : 1.0f);

      if (powerPelletTime == 0.0f && pacManPosition.x < 17.0f) { // eat power pellet
        powerPelletTime = maxPowerPelletTime;
        pacManDirection = DIRECTION_RIGHT;
      }
    } else {
      ghostEdiblePointVisibledTime -= elapsedTime;

      if (ghostEdiblePointVisibledTime <= 0.0f) {
        ghostEdiblePointVisibledTime = 0.0f;
        ghostEdiblePointGhost = -1;

        if (ghostEdiblePointIndex == numberOfGhosts) {
          finished = true;
          isAutoPlay = true;
        }
      }
    }

    for (int i = ghostEdiblePointIndex; i < numberOfGhosts && !finished; i++) {
      if (ghostEdiblePointVisibledTime == 0.0f) {
        ghostFrame[i] += elapsedTime * ghostAnimationSpeed;

        if (ghostFrame[i] >= numberOfGhostFrames) {
          ghostFrame[i] = 0.0f;
        }
      }

      ghostDirection[i] = pacManDirection;

      if (pacManDirection == DIRECTION_LEFT) {
        ghostPosition[i] = {pacManPosition.x + 13 + (i * (ghostSize.width + 1)), pacManPosition.y};
      } else {
        if (ghostEdiblePointVisibledTime == 0.0f) {
          ghostPosition[i].x += elapsedTime * (speed * 0.35f);

          if (isCollideGhost(i)) {
            ghostEdiblePointVisibledTime = 1000.0f;
            ghostEdiblePointString = String(ghostEdiblePoint[ghostEdiblePointIndex]);
            ghostEdiblePointSize = {(ghostEdiblePointString.length() * (numberSize.width - 1)) - 1, numberSize.height};
            ghostEdiblePointPosition = {ghostPosition[i].x + ((ghostSize.width - ghostEdiblePointSize.width) * 0.5f) - 3, ghostPosition[i].y + ((ghostSize.height - ghostEdiblePointSize.height) * 0.5f)};
            ghostEdiblePointIndex++;
            ghostEdiblePointGhost = i;
          }
        }
      }
    }
  } else if (titleTime < 5250.0f) {
    if (isSoundEnabled && (isGamepadEnabled && (pressedButton == BUTTON_UP || pressedButton == BUTTON_DOWN))) {
      if (pressedButton == BUTTON_UP) {
        increaseVolume();
      } else if (pressedButton == BUTTON_DOWN) {
        decreaseVolume();
      }

      titleTime = 250.0f;
    }
  }
}
