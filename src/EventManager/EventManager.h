#pragma once
#include "Event.h"
#include <queue>
#include <map>
#include <list>
#include <memory>

using namespace std;

class EventManager{

    public:

        ~EventManager() {};
        EventManager() {};
        static shared_ptr<EventManager> GetInstance();
        void Suscribe(Listener);
        void UnSuscribe(EventType, string);
        void AddEvent(Event);
        void Update();

    private:
        static const shared_ptr<EventManager> instance;
        void ShowSuscribers();
        //FIXME: Al final tendremos que escoger si usar la cola o la lista
        //std::queue<Event> eventQueue;
        std::list<Event> eventList;
        
        std::map<EventType, ListenerVector> eventListenerMap; //Mapa con el tipo de evento y un vector con los listeners suscritos a dicho evento

};