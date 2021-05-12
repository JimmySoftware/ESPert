// All in One Game for ESPresso Lite

#include "Menu.h"

ESPert espert;
Game* game = NULL;

void setup() {
  espert.init();
  game = new menu::Menu();
  game->init(&espert, false, true);
}

void loop() {
  game->update();
  game->render();
}
