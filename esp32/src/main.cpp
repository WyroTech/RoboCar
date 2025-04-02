#include <Arduino.h>
#include "camera.h"
#include "system.h"
#include "wifi_connection.h"
#include "web_socket.h"

Camera my_camera;
System my_system;
WifiConnection wifi_connection;
WebSocketHandler web_socket = WebSocketHandler(81, &my_camera);

void setup() {
    my_system.init();
    my_system.print_system_info();
    Serial.println("Init finished");

    wifi_connection.change_to_wifi_station_mode();
    wifi_connection.scan_wifi();
    wifi_connection.connect_to_wifi();

    my_camera.camera_init();
    my_camera.test_camera();

    web_socket.begin();

    Serial.println("Setup done");
}

void loop() {
    wifi_connection.loop();
    web_socket.loop();
    delay(10);
}



