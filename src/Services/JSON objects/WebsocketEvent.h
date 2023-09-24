#ifndef WEBSOCKET_EVENT_H_
#define WEBSOCKET_EVENT_H_

struct WebsocketEvent
{
    typedef enum
    {
        UNKNOWN = -1,
        OPEN_LOCK = 0,
        LOCK_IS_OPEN,
        LOCK_IS_CLOSED
    } EventTypeEnum;

    EventTypeEnum eventType;
};

struct OpenLockEvent: WebsocketEvent
{
    OpenLockEvent(WebsocketEvent::EventTypeEnum eventType, long expirationTime)
    {
        this->eventType = eventType;
        this->expirationTime = expirationTime;
    }
    long expirationTime;
};

#endif