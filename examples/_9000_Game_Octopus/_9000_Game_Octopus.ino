// Game & Watch Octopus Simulator for ESPresso Lite

#include "Octopus.h"

ESPert espert;
Game* game = NULL;

void setup() {
  espert.init();
  game = new octopus::Octopus();
  game->init(&espert);
}

void loop() {
  game->update();
  game->render();
}
