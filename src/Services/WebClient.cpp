#include "WebClient.h"

WebClient::WebClient() = default;

void WebClient::webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[WebClient] Disconnected!\n");
        lockInstance->setServerConnectionStatus(WStype_DISCONNECTED);
        lockInstance->ipAddress = NULL;
        break;
    case WStype_CONNECTED:
        Serial.printf("[WebClient] Connected to url: %s\n", payload);

        // send message to server when Connected
        websocketsClient.sendTXT("Connected");
        sendDeviceSerialNumberToServer();
        lockInstance->setServerConnectionStatus(WStype_CONNECTED);
        break;
    case WStype_TEXT:
    {
        Serial.printf("[WebClient] get text: %s\n", payload);
        // send message to server
        // webSocket.sendTXT("message here");
        WebsocketEvent websoketEvent = parsePayload(payload);
        handleEvent(&websoketEvent);
    }
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

void WebClient::setWebsocketConnection(const char *host, int port, const char *url, const char *rootCACert)
{
    this->websocketsClient.beginSslWithCA(host, port, url, rootCACert);
    std::function<void(WStype_t, uint8_t *, size_t)>
        callbackWrapper;
    callbackWrapper = [this](WStype_t type, uint8_t *payload, size_t length)
    {
        webSocketEvent(type, payload, length);
    };
    this->websocketsClient.onEvent(callbackWrapper);
}

void WebClient::sendDeviceSerialNumberToServer()
{
    if (lockInstance->deviceSerialNumber != NULL && lockInstance->ipAddress != NULL)
    {
        StaticJsonDocument<1024> doc;
        doc["serialNumber"] = lockInstance->deviceSerialNumber;
        doc["ip"] = lockInstance->ipAddress;
        String stringJSON = doc.as<String>();
        this->websocketsClient.sendTXT(stringJSON);
    }
}

WebsocketEvent WebClient::parsePayload(uint8_t *payload)
{
    WebsocketEvent websocketEvent = {.eventType = WebsocketEvent::UNKNOWN};

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error)
    {
        Serial.print(F("deserializeJson() returned "));
        Serial.println(error.f_str());
        return websocketEvent;
    }

    String eventTypeString = doc["eventType"];
    if (eventTypeString == "OPEN_LOCK")
    {
        WebsocketEvent::EventTypeEnum eventType = WebsocketEvent::OPEN_LOCK;
        unsigned long experationTime = doc["data"]["experationTime"];
        OpenLockEvent openLockEvent = OpenLockEvent(eventType, experationTime);
        websocketEvent = openLockEvent;
    }

    return websocketEvent;
}

void WebClient::handleEvent(const WebsocketEvent *event)
{
    switch (event->eventType)
    {
    case WebsocketEvent::OPEN_LOCK:
    {
        Serial.println("[WebClient] handle event type OPEN_LOCK");
        const OpenLockEvent *openLockEvent = reinterpret_cast<const OpenLockEvent *>(event);
        time_t currentTimeInSeconds;
        time(&currentTimeInSeconds);
        if (currentTimeInSeconds < openLockEvent->expirationTime)
        {
            lockInstance->openLock();
        }
    }
    break;
    default:
        break;
    }
    return;
}

void WebClient::loop()
{
    this->websocketsClient.loop();
}