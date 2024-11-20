//
// Created by Andreas Wyrobek on 20.11.24.
//

#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H

#include <WebSocketsClient.h>

class WebSocket {
public:
    void init();

    void loop();

private:
    WebSocketsClient webSocket;
    const char *server_ip = "192.168.2.209";

    static void onEvent(WStype_t type, uint8_t *payload, size_t length);

    void send_image();
};


#endif //WEB_SOCKET_H
