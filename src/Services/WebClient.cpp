#include "WebClient.h"

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

WebClient::WebClient()
{
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    setenv("TZ", "UTC0", 1);
    tzset();
};

void WebClient::webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[WebClient] Disconnected!\n");
        lockInstance->setServerConnectionStatus(NULL, WStype_DISCONNECTED);
        this->ipAddress = NULL;
        break;
    case WStype_CONNECTED:
        Serial.printf("[WebClient] Connected to url: %s\n", payload);

        // send message to server when Connected
        websocketsClient.sendTXT("Connected");
        lockInstance->setServerConnectionStatus(this, WStype_CONNECTED);
        break;
    case WStype_TEXT:
    {
        // send message to server
        // webSocket.sendTXT("message here");
        WebsocketEvent *websoketEvent = new WebsocketEvent();
        parsePayload(payload, websoketEvent);
        handleEvent(websoketEvent);
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
    std::string serialNumerHeader = "SerialNumber: ";
    serialNumerHeader.append(lockInstance->deviceSerialNumber);
    this->websocketsClient.setExtraHeaders(serialNumerHeader.c_str());

    std::function<void(WStype_t, uint8_t *, size_t)>
        callbackWrapper;
    callbackWrapper = [this](WStype_t type, uint8_t *payload, size_t length)
    {
        webSocketEvent(type, payload, length);
    };
    this->websocketsClient.onEvent(callbackWrapper);
    this->websocketsClient.beginSslWithCA(host, port, url, rootCACert);
}

void WebClient::parsePayload(uint8_t *payload, WebsocketEvent * returnValue)
{
    WebsocketEvent *websocketEvent = new WebsocketEvent();
    websocketEvent->eventType = WebsocketEvent::UNKNOWN;

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error)
    {
        Serial.print(F("deserializeJson() returned "));
        Serial.println(error.f_str());
    }

    String eventTypeString = doc["eventType"];
    if (eventTypeString == "OPEN_LOCK")
    {
        WebsocketEvent::EventTypeEnum eventType = WebsocketEvent::OPEN_LOCK;
        long expirationTime = doc["data"]["expirationTime"];
        OpenLockEvent *openLockEvent = new OpenLockEvent(eventType, expirationTime);
        websocketEvent = openLockEvent;
    }

    memcpy(returnValue, (const WebsocketEvent*)websocketEvent, sizeof(websocketEvent));
}

void WebClient::handleEvent(WebsocketEvent *event)
{
    switch (event->eventType)
    {
    case WebsocketEvent::OPEN_LOCK:
    {
        Serial.println("[WebClient] handle event type OPEN_LOCK");
        OpenLockEvent *openLockEvent = (OpenLockEvent*)event;
        unsigned long currentTime = getCurrentUTC0Time();
        if (currentTime < openLockEvent->expirationTime)
        {
            lockInstance->openLock();
        }
    }
    break;
    case WebsocketEvent::LOCK_IS_OPEN:
    {
        DynamicJsonDocument doc(1024);
        doc["eventType"] = "LOCK_IS_OPEN";
        long currentTime = getCurrentUTC0Time();
        doc["data"]["openTime"] = currentTime;
        String stringJSON = doc.as<String>();
        websocketsClient.sendTXT(stringJSON);
    }
    break;
    case WebsocketEvent::LOCK_IS_CLOSED:
    {
        DynamicJsonDocument doc(1024);
        doc["eventType"] = "LOCK_IS_CLOSED";
        long currentTime = getCurrentUTC0Time();
        doc["data"]["closedTime"] = currentTime;
        String stringJSON = doc.as<String>();
        websocketsClient.sendTXT(stringJSON);
    }
    break;
    default:
        break;
    }
    return;
}

unsigned long WebClient::getCurrentUTC0Time()
{
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    //Serial.println(&timeinfo, "[WebClient] Current time: %A, %B %d %Y %H:%M:%S");
    time_t unixSystemTime = mktime(&timeinfo);
    unsigned long returnValue = unixSystemTime;
    return returnValue;
}

void WebClient::loop()
{
    this->websocketsClient.loop();
}