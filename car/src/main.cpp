#include <Arduino.h>
#include "system.h"

System my_system;

void setup() {
  my_system.init();
}

void loop() {
  my_system.loop();
}