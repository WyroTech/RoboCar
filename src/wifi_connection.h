//
// Created by Andreas Wyrobek on 20.11.24.
//

#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H


class WifiConnection {
public:
    void change_to_wifi_station_mode();

    void scan_wifi();

    void connect_to_wifi();

    bool is_connected();

    void check_for_wifi_connection_and_reconnect();

    void loop();

private:
    const char *ssid = "WLAN-686386";
    const char *password = "78744264384698314996";
    IPAddress staticIP = IPAddress(192, 168, 2, 123); // Your static IP
    IPAddress gateway = IPAddress(192, 168, 2, 1); // Your gateway IP
    IPAddress subnet = IPAddress(255, 255, 255, 0); // Your subnet mask
    IPAddress dns = IPAddress(8, 8, 8, 8); // DNS (Google)

    unsigned long timeout = 20000; // 20 seconds timeout
    unsigned long reconnectInterval = 5000; // 30 seconds between
    unsigned long previousMillis = 0;
};


#endif //WIFI_CONNECTION_H
