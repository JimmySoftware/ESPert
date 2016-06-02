// Pac-Man Simulator for ESPresso Lite

#include "PacMan.h"

ESPert espert;
Game* game = NULL;

void setup() {
  espert.init();
  game = new pacMan::PacMan();
  game->init(&espert);
}

void loop() {
  game->update();
  game->render();
}
