#include "WebClient.h"

WebClient::WebClient() = default;

void WebClient::webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[WebClient] Disconnected!\n");
        lockInstance->setServerConnectionStatus(WStype_DISCONNECTED);
        break;
    case WStype_CONNECTED:
        Serial.printf("[WebClient] Connected to url: %s\n", payload);
        lockInstance->setServerConnectionStatus(WStype_CONNECTED);
        // send message to server when Connected
        websocketsClient.sendTXT("Connected");
        break;
    case WStype_TEXT:
        Serial.printf("[WebClient] get text: %s\n", payload);
        lockInstance->openLock();
        // send message to server
        // webSocket.sendTXT("message here");
        break;
    case WStype_BIN:
        Serial.printf("[WebClient] get binary length: %u\n", length);

        // send data to server
        // webSocket.sendBIN(payload, length);
        break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
        break;
    }
}

void WebClient::setWebsocketConnection(const char *ip, int port, const char *url)
{
    this->websocketsClient.begin(ip, port, url);
    std::function<void(WStype_t, uint8_t *, size_t)> callbackWrapper;
    callbackWrapper = [this](WStype_t type, uint8_t *payload, size_t length)
    {
        webSocketEvent(type, payload, length);
    };
    this->websocketsClient.onEvent(callbackWrapper);
}

void WebClient::loop()
{
    this->websocketsClient.loop();
}