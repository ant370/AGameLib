#ifndef ANT_EVENTS
#define ANT_EVENTS



struct GameEvent
{

    int eventId;
    void * event;
};

#define GAMEEVENT_RESOURCELOADED 100
struct GameEvent_ResourceLoaded
{
    int eventId;
    char * data;
};


void GameAdd(GameEvent event);

void GameLoop(GameEvent event)
{
    GameEvent_ResourceLoaded * resc = (GameEvent_ResourceLoaded *) event.event;
    
    // do whatever.
}

#endif