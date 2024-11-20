#include "web_socket.h"

WebSocketHandler *WebSocketHandler::instance = nullptr;

WebSocketHandler::WebSocketHandler(uint16_t port) : webSocket(port) {
        instance = this;
}

void WebSocketHandler::begin() {
        webSocket.begin();
        webSocket.onEvent(WebSocketHandler::webSocketEvent);
        Serial.println("WebSocket server started");
}

void WebSocketHandler::loop() {
        webSocket.loop();
}

void WebSocketHandler::sendMessage(uint8_t num, String &message) {
        webSocket.sendTXT(num, message);
}

void WebSocketHandler::webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
        if (instance) {
                switch (type) {
                        case WStype_DISCONNECTED:
                                instance->handleDisconnection(num);
                                break;

                        case WStype_CONNECTED:
                                instance->handleConnection(num, instance->webSocket.remoteIP(num));
                                break;

                        case WStype_TEXT:
                                instance->handleMessage(num, payload, length);
                                break;
                }
        }
}

void WebSocketHandler::handleDisconnection(uint8_t num) {
        Serial.printf("[%u] Disconnected!\n", num);
}

void WebSocketHandler::handleConnection(uint8_t num, IPAddress ip) {
        Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
}

void WebSocketHandler::handleMessage(uint8_t num, uint8_t *payload, size_t length) {
        String message = String((char *) payload);
        Serial.printf("[%u] Received text: %s\n", num, message.c_str());

        // Echo the message back to the client
        sendMessage(num, message);
}
