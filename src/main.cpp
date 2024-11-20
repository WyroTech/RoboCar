#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include "esp_camera.h"
#include "esp_system.h"


// Camera pins for ESP32-S3
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22


bool initCamera() {
    // Disable PSRAM
    pinMode(PWDN_GPIO_NUM, OUTPUT);
    digitalWrite(PWDN_GPIO_NUM, LOW);
    delay(10);
    digitalWrite(PWDN_GPIO_NUM, HIGH);
    delay(10);

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    // Minimal configuration for non-PSRAM boards
    config.xclk_freq_hz = 20000000; // 8MHz XCLK
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_QQVGA; // 160x120 resolution
    config.jpeg_quality = 12; // Lower quality (0-63)
    config.fb_count = 1; // Minimum frame buffers

    // Initialize camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return false;
    }

    Serial.println("Camera initialized successfully!");

    // Get sensor settings
    sensor_t *s = esp_camera_sensor_get();
    if (s) {
        // Apply lowest possible settings
        s->set_framesize(s, FRAMESIZE_QQVGA);
        s->set_quality(s, 12);
        s->set_brightness(s, 0);
        s->set_contrast(s, 0);
        s->set_saturation(s, 0);
    }

    return true;
}

void testingCamara() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera init failed!");
        return;
    } else {
        Serial.println("Camera init successful!");
    }
}

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
        /*camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
            delay(1000);
            return;
        }

        webSocket.sendBIN(fb->buf, fb->len);
        esp_camera_fb_return(fb);
        */
        //webSocket.sendTXT("Hello World!");
    }
}

void setup() {
    Serial.begin(SERIAL_CHANNEL);
    delay(100);
    Serial.println("Starting up");

    initCamera();
    testingCamara();

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

