//
// Created by Andreas Wyrobek on 22.11.24.
//

#include "voltage.h"

void Voltage::init()
{
  pinMode(PIN_Voltage, INPUT);
}

float Voltage::getAnalogue()
{
  float Voltage = (analogRead(PIN_Voltage) * 0.0375);
  Voltage = Voltage + (Voltage * 0.08); //补偿8%
  return Voltage;
}