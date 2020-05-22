/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #pragma once
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <thread>
#include <mutex>
#include "Event.h"

//using namespace std;

class EventManager {
   public:
    EventManager(EventManager const&) = delete;
    void operator=(EventManager const&) = delete;
    ~EventManager(){};

    static EventManager& GetInstance();
    void SubscribeMulti(const Listener&);
    void Subscribe(const Listener&);
    void UnSubscribeMulti(EventType, const string&);
    void AddEventMulti(const Event&);
    void Update();
    void ClearEvents();
    void ClearListeners();

   private:
    EventManager() : id_single(0) {  };

    std::thread::id idHiloCero{0};
    std::thread::id idOcupado {idHiloCero};

    uint32_t id_single;
    // static const EventManager instance;
    void ShowSuscribers();
    //FIXME: Al final tendremos que escoger si usar la cola o la lista
    //std::queue<Event> eventQueue;
    list<Event> events;  // lista de eventos posibles

    map<EventType, ListenerVector> listeners;  //Mapa con el tipo de evento y un vector con los listeners suscritos a dicho evento
    std::mutex mutex_events;
    // std::mutex mutex_update;
};