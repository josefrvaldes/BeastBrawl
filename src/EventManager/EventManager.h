#pragma once
#include <list>
#include <map>
#include <memory>
#include <queue>
#include "Event.h"
// #include <include/tbb/concurrent_vector.h>
#include "../../include/tbb/concurrent_queue.h"
#include "../../include/tbb/concurrent_vector.h"
// #include "../../include/tbb/concurrent_map.h"

using namespace std;

class EventManager {
   public:
    EventManager(EventManager const&) = delete;
    void operator=(EventManager const&) = delete;
    ~EventManager(){};

    static EventManager& GetInstance();
    void SuscribeMulti(Listener);
    void Suscribe(Listener);
    void UnSuscribeMulti(EventType, string);
    void AddEventMulti(Event);
    void Update();
    void ClearEvents();
    void ClearListeners();

   private:
    EventManager() : id_single(0) {  };

    uint32_t id_single;
    // static const EventManager instance;
    void ShowSuscribers();
    //FIXME: Al final tendremos que escoger si usar la cola o la lista
    //std::queue<Event> eventQueue;
    // list<Event> eventList;  // lista de eventos posibles
    tbb::concurrent_vector<Event> eventList;  // lista de eventos posibles
    // tbb::concurrent_queue<Event> eventList;  // lista de eventos posibles

    map<EventType, ListenerVector> eventListenerMap;  //Mapa con el tipo de evento y un vector con los listeners suscritos a dicho evento
    // tbb::concurrent_map<EventType, ListenerVector> eventListenerMap;  //Mapa con el tipo de evento y un vector con los listeners suscritos a dicho evento
};