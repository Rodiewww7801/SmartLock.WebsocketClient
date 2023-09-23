#ifndef WEBCLIENT_H_
#define WEBCLIENT_H_

#include <WebSocketsClient.h>
#include "LockInstance.h"
#include <ArduinoJson.h>
#include "JSON objects/WebsocketEvent.h"

class WebClient
{
private:
    WebSocketsClient websocketsClient;
    void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
    void sendDeviceSerialNumberToServer();
    WebsocketEvent parsePayload(uint8_t *payload);
    void handleEvent(const WebsocketEvent *event);

public:
    LockInstance *lockInstance = NULL;

    WebClient();
    void setWebsocketConnection(const char *host, int port, const char *url = "/", const char *rootCACert = "");
    void loop();
};

#endif