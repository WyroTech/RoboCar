//
// Created by Andreas Wyrobek on 20.11.24.
//

#include "system.h"
#include "esp_system.h"
#include <Arduino.h>

#define RXD2 3
#define TXD2 40

void System::print_system_info() {
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    Serial.println("ESP32 Chip Information:");
    Serial.printf("Model: %d\n", chip_info.model);
    Serial.printf("Features: %d\n", chip_info.features);
    Serial.printf("Cores: %d\n", chip_info.cores);
    Serial.printf("Revision: %d\n", chip_info.revision);

    // Check if PSRAM is actually enabled
    if (psramFound()) {
        Serial.println("PSRAM is enabled");
        Serial.printf("Total PSRAM: %d bytes\n", ESP.getPsramSize());
        Serial.printf("Free PSRAM: %d bytes\n", ESP.getFreePsram());
    } else {
        Serial.println("PSRAM is not enabled");
    }
}

void System::init() {
    Serial.begin(SERIAL_CHANNEL);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    delay(1000);
    Serial.println("Starting up");
}
