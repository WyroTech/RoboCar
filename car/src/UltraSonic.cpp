//
// Created by Andreas Wyrobek on 22.11.24.
//

#include "UltraSonic.h"
#include <Arduino.h>

void UltraSonic::init(void)
{
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
}

int UltraSonic::get()
{
  unsigned int tempda_x = lastValue;
  if (!timer.isReady()) {
    return tempda_x;
  }
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  tempda_x = ((unsigned int)pulseIn(ECHO_PIN, HIGH) / 58);

  if (tempda_x > 150)
  {
    lastValue = 150;
    return 150;
  }
  else
  {
    lastValue = tempda_x;
    return tempda_x;
  }
}