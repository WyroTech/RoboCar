//
// Created by Andreas Wyrobek on 22.11.24.
//

#ifndef ULTRASONIC_H
#define ULTRASONIC_H
#include "Timer.cpp"


class UltraSonic {
public:
  void init();
  int get();

private:
  Timer timer = Timer(50);
  int lastValue = 0;
#define TRIG_PIN 13      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 12      // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
};

#endif //ULTRASONIC_H
