// Flappy Bird Simulator for ESPresso Lite

#include "FlappyBird.h"

ESPert espert;
Game* game = NULL;

void setup() {
  espert.init();
  game = new flappyBird::FlappyBird();
  game->init(&espert);
}

void loop() {
  game->update();
  game->render();
}
