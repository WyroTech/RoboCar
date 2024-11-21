#ifndef WEBSOCKET_HANDLER_H
#define WEBSOCKET_HANDLER_H

#include <Arduino.h>
#include <camera.h>
#include <WebSocketsServer.h>
#include <WiFi.h>

#include "../../../.platformio/packages/toolchain-riscv32-esp/riscv32-esp-elf/include/c++/8.4.0/list"

class WebSocketHandler {
public:
    WebSocketHandler(uint16_t port = 81, Camera *camera = nullptr);

    void begin();

    void loop();

    void sendMessage(uint8_t num, String &message);

    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

private:
    static WebSocketHandler *instance;
    Camera *camera;
    WebSocketsServer webSocket;
    std::list<uint8_t> connectedClients = std::list<uint8_t>();

    // Private event handlers
    void handleDisconnection(uint8_t num);

    void handleConnection(uint8_t num, IPAddress ip);

    void handleMessage(uint8_t num, uint8_t *payload, size_t length);

    void sendImage(uint8_t num);
};

#endif
