#ifndef LOCKINSTANCE_H_
#define LOCKINSTANCE_H_

#include <Arduino.h>
#include <WebSocketsClient.h>

typedef enum
{
    Open,
    Closed,
} LockInstanceSatus;

class LockInstance
{
private:
    unsigned long timerStartLine;
    LockInstanceSatus lockStatus;
    WStype_t serverConnectionStatus;

public:
    const char *deviceSerialNumber = NULL;
    const char *ipAddress = NULL;
    int8_t currentGPIO = GPIO_NUM_NC;
    int8_t serverStatusGPIO = GPIO_NUM_NC;
    unsigned long timerDeadline;

    LockInstance();
    LockInstanceSatus getLockStatus();
    unsigned long getTimerStartLine();
    void setServerConnectionStatus(WStype_t serverConnectionStatus);
    WStype_t getServerConnectionStatus();
    void openLock();
    void closeLock();
    void loop();
};

#endif