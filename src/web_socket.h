#ifndef WEBSOCKET_HANDLER_H
#define WEBSOCKET_HANDLER_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include <WiFi.h>

class WebSocketHandler {
public:
    WebSocketHandler(uint16_t port = 81);

    void begin();

    void loop();

    void sendMessage(uint8_t num, String &message);

    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

private:
    static WebSocketHandler *instance;
    WebSocketsServer webSocket;

    // Private event handlers
    void handleDisconnection(uint8_t num);

    void handleConnection(uint8_t num, IPAddress ip);

    void handleMessage(uint8_t num, uint8_t *payload, size_t length);
};

#endif
