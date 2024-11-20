//
// Created by Andreas Wyrobek on 20.11.24.
//

#include <WiFi.h>
#include "wifi_connection.h"

void WifiConnection::change_to_wifi_station_mode() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Change To WiFi Station Mode");
}

void WifiConnection::scan_wifi() {
    Serial.println("Scanning for WiFi networks...");
    int networkCount = WiFi.scanNetworks();
    if (networkCount != 0) {
        // Print details of each network
        for (int i = 0; i < networkCount; ++i) {
            if (!WiFi.SSID(i).equals(ssid)) continue;
            // Print SSID and RSSI for each network
            Serial.print("Found ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(" dBm)");
            return;
        }
    }
    Serial.println("No matching WiFi found called ");
    Serial.println(ssid);
    return;
}

void WifiConnection::connect_to_wifi() {
    Serial.print("\nConnecting to ");
    Serial.print(ssid);
    Serial.print("...");

    WiFi.config(staticIP, gateway, subnet, dns);
    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected successfully!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nConnection failed!");
    }

    delay(1000);
}

bool WifiConnection::is_connected() {
  return WiFi.status() == WL_CONNECTED;
}

void WifiConnection::check_for_wifi_connection_and_reconnect() {
    if (is_connected() == false) {
        Serial.println("[WARNING] WiFi connection lost!");
        WiFi.disconnect();
        delay(1000);
        connect_to_wifi();
    } else {
        Serial.println("WiFi is connected!");
    }
}

void WifiConnection::loop() {
    const unsigned long currentMillis = millis();
    unsigned long overReconnectInterval = currentMillis - previousMillis;

    if (overReconnectInterval >= reconnectInterval) {
        previousMillis = currentMillis;
        check_for_wifi_connection_and_reconnect();
    }
}