#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include "esp_camera.h"

constexpr int SERIAL_CHANNEL = 115200;

auto ssid = "WLAN-686386";
auto password = "78744264384698314996";
auto websocketServer = "192.168.2.209";
IPAddress staticIP(192, 168, 2, 123); // Your static IP
IPAddress gateway(192, 168, 2, 1); // Your gateway IP
IPAddress subnet(255, 255, 255, 0); // Your subnet mask
IPAddress dns(8, 8, 8, 8); // DNS (Google)

WebSocketsClient webSocket;

constexpr unsigned long timeout = 20000; // 20 seconds timeout
constexpr unsigned long reconnectInterval = 30000; // 30 seconds between
unsigned long previousMillis = 0;

void changeToWiFiStationMode() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Change To WiFi Station Mode");
}

bool scanWiFi() {
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
            return true;
        }
    }
    Serial.println("No matching WiFi found called ");
    Serial.println(ssid);
    return false;
}

void connectToWiFi() {
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
}

void checkWiFiConnectionAndReconnect() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[WARNING] WiFi connection lost!");
        WiFi.disconnect();
        delay(1000);
        connectToWiFi();
    }
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.println("Websocket disconnected!");
            break;
        case WStype_CONNECTED:
            Serial.println("Websocket Connected!");
            break;
        default:
            break;
    }
}

void setupWebSocket() {
    Serial.print("Websocket setup");
    Serial.print(websocketServer);
    Serial.println("...");
    webSocket.begin(websocketServer, 8080, "/", "ws");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
}

void sendImageWS() {
    if (webSocket.isConnected()) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
            delay(1000);
            return;
        }

        webSocket.sendBIN(fb->buf, fb->len);
        esp_camera_fb_return(fb);
    }
}

void setup() {
    Serial.begin(SERIAL_CHANNEL);

    changeToWiFiStationMode();
    scanWiFi();
    connectToWiFi();
    if (WiFi.status() == WL_CONNECTED) {
        setupWebSocket();
    }
    Serial.println("Setup done");
}

void loop() {
    const unsigned long currentMillis = millis();
    unsigned long overReconnectInterval = currentMillis - previousMillis;
    if (overReconnectInterval >= reconnectInterval) {
        previousMillis = currentMillis;
        checkWiFiConnectionAndReconnect();
    }

    webSocket.loop();
    if (webSocket.isConnected()) {
        sendImageWS();
        delay(100); // Adjust based on your needs
    }
}
