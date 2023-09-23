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
    OpenLockEvent(WebsocketEvent::EventTypeEnum eventType, unsigned long experationTime)
    {
        this->eventType = eventType;
        this->experationTime = experationTime;
    }
    unsigned long experationTime;
};