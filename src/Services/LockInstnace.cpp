#include "LockInstance.h"
#include "WebClient.h"

LockInstance::LockInstance()
{
    this->timerStartLine = 0;
    this->timerDeadline = 5000;
    this->lockStatus = Closed;
    this->serverConnectionStatus = WStype_DISCONNECTED;
}

LockInstanceSatus LockInstance::getLockStatus()
{
    return this->lockStatus;
}

unsigned long LockInstance::getTimerStartLine()
{
    return this->timerStartLine;
}

void LockInstance::setServerConnectionStatus(WebClient *webClient, WStype_t serverConnectionStatus)
{
    this->webClient = webClient;
    this->serverConnectionStatus = serverConnectionStatus;
    if (serverConnectionStatus == WStype_CONNECTED)
    {
        digitalWrite(serverStatusGPIO, HIGH);
        Serial.printf("[LockInstance] Turned ON GPIO %d\n", serverStatusGPIO);
    }
    else if (serverConnectionStatus == WStype_DISCONNECTED)
    {
        digitalWrite(serverStatusGPIO, LOW);
        Serial.printf("[LockInstance] Turned OFF GPIO %d\n", serverStatusGPIO);
        closeLock();
    }
}

WStype_t LockInstance::getServerConnectionStatus()
{
    return this->serverConnectionStatus;
}

void LockInstance::openLock()
{
    if (lockStatus == Closed)
    {
        digitalWrite(currentGPIO, HIGH);
        timerStartLine = millis();
        lockStatus = Open;

        Serial.println("[LockInstance] Lock is open");
        Serial.printf("[LockInstance] Turned ON GPIO %d\n", currentGPIO);

        WebsocketEvent websocketEvent = {.eventType = WebsocketEvent::LOCK_IS_OPEN};
        if (webClient != NULL)
        {
            webClient->handleEvent(&websocketEvent);
        }
    }
}

void LockInstance::closeLock()
{
    if (digitalRead(currentGPIO) == HIGH)
    {
        digitalWrite(currentGPIO, LOW);
        timerStartLine = 0;
        lockStatus = Closed;

        Serial.println("[LockInstance] Lock is closed");
        Serial.printf("[LockInstance] Turned OFF GPIO %d\n", currentGPIO);

        WebsocketEvent websocketEvent = WebsocketEvent(WebsocketEvent::LOCK_IS_CLOSED);
        if (webClient != NULL)
        {
            webClient->handleEvent(&websocketEvent);
        }
    }
}

void LockInstance::loop()
{
    if (lockStatus == Open)
    {
        if (millis() > timerStartLine + timerDeadline)
        {
            closeLock();
        }
    }
}