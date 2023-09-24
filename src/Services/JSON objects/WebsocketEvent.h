struct WebsocketEvent
{
    typedef enum
    {
        UNKNOWN = -1,
        OPEN_LOCK = 0
    } EventTypeEnum;

    EventTypeEnum eventType;
};

struct OpenLockEvent: WebsocketEvent
{
    OpenLockEvent(WebsocketEvent::EventTypeEnum eventType, unsigned long exparationTime)
    {
        this->eventType = eventType;
        this->expirationTime = exparationTime;
    }
    unsigned long expirationTime;
};