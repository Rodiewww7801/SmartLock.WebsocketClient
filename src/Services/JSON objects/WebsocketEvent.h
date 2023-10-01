#ifndef WEBSOCKET_EVENT_H_
#define WEBSOCKET_EVENT_H_

class WebsocketEvent
{
public:
    typedef enum
    {
        UNKNOWN = -1,
        OPEN_LOCK = 0,
        LOCK_IS_OPEN,
        LOCK_IS_CLOSED
    } EventTypeEnum;

    EventTypeEnum eventType;

    WebsocketEvent() = default;
    WebsocketEvent(EventTypeEnum eventType)
    {
        this->eventType = eventType;
    }

    WebsocketEvent(const WebsocketEvent &event)
    {
        this->eventType = event.eventType;
    }
};

class OpenLockEvent : public WebsocketEvent
{
public:
    long expirationTime;
    OpenLockEvent(WebsocketEvent::EventTypeEnum eventType, long expirationTime)
    {
        this->eventType = eventType;
        this->expirationTime = expirationTime;
    }

    OpenLockEvent(const OpenLockEvent &event) : WebsocketEvent(event)
    {
        this->expirationTime = event.expirationTime;
    }
};

#endif