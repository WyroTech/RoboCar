#ifndef TIMER
#define TIMER
#include <Arduino.h>

class Timer {
    unsigned long previousMillis;
    unsigned long interval;

public:
    Timer(unsigned long interval) : interval(interval) {
        previousMillis = 0;
    }

    bool isReady() {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            return true;
        }
        return false;
    }
};

#endif