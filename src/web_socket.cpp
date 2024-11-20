//
// Created by Andreas Wyrobek on 20.11.24.
//

#include "web_socket.h"
#include "esp_camera.h"

void WebSocket::init() {
        Serial.print("Connect with Websocket ");
        Serial.print(server_ip);
        Serial.println("...");
        webSocket.disableHeartbeat();
        webSocket.begin(server_ip, 8080, "/", "ws");
        webSocket.onEvent(onEvent);
        webSocket.setReconnectInterval(5000);
}

void WebSocket::onEvent(WStype_t type, uint8_t *payload, size_t length) {
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

void WebSocket::loop() {
        webSocket.loop();

        if (webSocket.isConnected()) {
          send_image();
        }
}

void WebSocket::send_image() {
                camera_fb_t *fb = esp_camera_fb_get();
                webSocket.sendBIN(fb->buf, fb->len);
                esp_camera_fb_return(fb);

}