//
// Created by Andreas Wyrobek on 22.11.24.
//

#include "system.h"
#include <Arduino.h>
#include <ArduinoJson.h>

enum COMMAND {
    STOP = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4,
    UP_LEFT = 5,
    UP_RIGHT = 6,
    DOWN_LEFT = 7,
    DOWN_RIGHT = 8,
};

void System::init() {
    Serial.begin(SERIAL_CHANNEL);
    Serial.println("Arduino Serial Communication Started");
    motor.init();
    ultraSonic.init();
}


void System::loop() {
    if (!Serial.available()) {
        switch (lastCommand) {
            case UP:
            case UP_LEFT:
            case UP_RIGHT:
                if (ultraSonic.get() < minDistance) {
                    motor.drive(direction_void, 0, direction_void, 0, control_enable);
                    lastCommand = STOP;
                }
            break;
        }

        return;
    }

    const String json = Serial.readStringUntil('\n');
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        return;
    }

    int command = doc["command"];
    int speed = doc["speed"];
    const int curving = 0.75;
    const int curveSpeed = speed * curving;
    Serial.println(command);
    Serial.println(ultraSonic.get());
    switch (command) {
        case STOP:
            motor.drive(direction_void, 0, direction_void, 0, control_enable);
        break;
        case UP:
            if (ultraSonic.get() < minDistance) return;
            motor.drive(direction_just, speed, direction_just, speed, control_enable);
            break;
        case DOWN:
            motor.drive(direction_back, speed, direction_back, speed, control_enable);
            break;
        case LEFT:
            motor.drive(direction_just, speed, direction_back, speed, control_enable);
            break;
        case RIGHT:
            motor.drive(direction_back, speed, direction_just, speed, control_enable);
            break;
        case UP_LEFT:
            if (ultraSonic.get() < minDistance) return;
            motor.drive(direction_just, speed, direction_just, curveSpeed, control_enable);
            break;
        case UP_RIGHT:
            if (ultraSonic.get() < minDistance) return;
            motor.drive(direction_just, curveSpeed, direction_just, speed, control_enable);
            break;
        case DOWN_LEFT:
            motor.drive(direction_back, curveSpeed, direction_back, speed, control_enable);
            break;
        case DOWN_RIGHT:
            motor.drive(direction_back, speed, direction_back, curveSpeed, control_enable);
            break;
    }
    lastCommand = command;
}