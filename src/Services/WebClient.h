#ifndef WEBCLIENT_H_
#define WEBCLIENT_H_

#include <WebSocketsClient.h>
#include "LockInstance.h"
#include <ArduinoJson.h>
#include "JSON objects/WebsocketEvent.h"
#include "time.h"

class WebClient
{
private:
    WebSocketsClient websocketsClient;
    void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
    void sendDeviceSerialNumberToServer();
    void parsePayload(uint8_t *payload, WebsocketEvent* returnValue);

public:
    LockInstance *lockInstance = NULL;
    const char *ipAddress = NULL;

    WebClient();
    void setWebsocketConnection(const char *host, int port, const char *url = "/", const char *rootCACert = "");
    void handleEvent(WebsocketEvent *event);
    unsigned long getCurrentUTC0Time();
    void loop();
};

#endif