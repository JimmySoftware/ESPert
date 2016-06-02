// Breakout Simulator for ESPresso Lite

#include "Breakout.h"

ESPert espert;
Game* game = NULL;

void setup() {
  espert.init();
  game = new breakout::Breakout();
  game->init(&espert);
}

void loop() {
  game->update();
  game->render();
}
