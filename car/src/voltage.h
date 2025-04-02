//
// Created by Andreas Wyrobek on 22.11.24.
//

#ifndef VOLTAGE_H
#define VOLTAGE_H

#include <Arduino.h>

class Voltage {
public:
  void init();
  float getAnalogue();
private:
  #define PIN_Voltage A3
};

#endif //VOLTAGE_H
