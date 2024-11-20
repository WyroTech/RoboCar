#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include "esp_camera.h"
#include "camera.h"
#include "system.h"
#include "wifi_connection.h"
#include "web_socket.h"

Camera my_camera;
System my_system;
WifiConnection wifi_connection;
WebSocket web_socket;

void setup() {
    my_system.init();
    my_system.print_system_info();

    my_camera.camera_init();
    my_camera.test_camera();

    wifi_connection.change_to_wifi_station_mode();
    wifi_connection.connect_to_wifi();

    web_socket.init();

    Serial.println("Setup done");
}

void loop() {
    wifi_connection.loop();
    web_socket.loop();
    delay(1000);
}



