#include "Event.h"

#include <queue>
#include <map>
#include <list>

class EventManager{

    public:

        EventManager() {}
        ~EventManager() {}
        static EventManager& GetInstance();
        void Suscribe(Listener);
        void AddEvent(Event);
        void Update();

    private:
        std::queue<Event> eventQueue;
        std::list<Event> eventList;
        
        std::map<EventType, ListenerVector> eventListenerMap;

};