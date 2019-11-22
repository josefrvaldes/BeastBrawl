#include "Event.h"

#include <queue>
#include <map>
#include <list>

class EventManager{

    public:

        EventManager() {}
        ~EventManager() {}
        static EventManager& GetInstance();
        void Suscribe(const Listener);
        void AddEvent(Event);
        void Update();

    private:
        //FIXME: Al final tendremos que escoger si usar la cola o la lista
        std::queue<Event> eventQueue;
        std::list<Event> eventList;
        
        std::map<EventType, ListenerVector> eventListenerMap; //Mapa con el tipo de evento y un vector con los listeners suscritos a dicho evento

};