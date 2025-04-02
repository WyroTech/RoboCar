//
// Created by Andreas Wyrobek on 22.11.24.
//

#ifndef SYSTEM_H
#define SYSTEM_H
#include "motor.h"
#include "UltraSonic.h"


class System {
public:
    void init();
    void loop();
private:
    const int SERIAL_CHANNEL = 9600;
    Motor motor = Motor();
    UltraSonic ultraSonic = UltraSonic();
    int lastCommand = -1;
    const int minDistance = 30;
};



#endif //SYSTEM_H
