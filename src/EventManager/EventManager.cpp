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
 
 
 #include "EventManager.h"

#include <iostream>
#include <mutex>
#include <thread>

// const shared_ptr<EventManager> EventManager::instance = make_shared<EventManager>();
// shared_ptr<EventManager> EventManager::IGetInstance() {
//     //static EventManager instance;
//     // if(instance==nullptr){
//     //     instance = make_shared<EventManager>();
//     // }
//     return instance;
// }

EventManager& EventManager::GetInstance() {
    static EventManager instance;
    return instance;
}

//Realiza y vacia todos los eventos que estaban añadidos
//O(n)
//Acceso: O(1)
void EventManager::Update() {
    // std::lock_guard<std::mutex> lock(mutex_events);
    // cout << "En este frame hay " << events.size() << " eventos" << endl;
    while (!events.empty()) {
        mutex_events.lock();
        Event e = events.front();  //Cojemos el primero en la lista
        events.pop_front();        // Lo sacamos de la lista
        mutex_events.unlock();

        //std::cout << "Procesando evento con prioridad: " << e.type << std::endl;

        //Tratamos el evento
        auto mapByType = listeners.find(e.type);
        if (mapByType != listeners.end()) {
            auto eventVector = mapByType->second;  // El vector de listeners del mapa segun el EventType

            for (const Listener& listener : eventVector) {
                listener.callback(e.data.get());
            }
        }
    }
}

//Añade un evento a la cola de eventos
// O(n) -> Recorremos la lista iterativamente hasta encontrar el valor directamente superior
void EventManager::AddEventMulti(const Event& e) {
    std::lock_guard<std::mutex> lock(mutex_events);
    //FIXME: Descomentar esto para que funcione con cola
    //eventQueue.push(e);

    //Si es el primer evento lo añadimos al comienzo
    if (events.empty()) {
        events.push_front(e);
    } else {
        //TODO: Implementar un algoritmo de busqueda e insercion mas optimo
        //Movemos el iterator hasta posicionarlo antes del siguiente valor mas grande
        std::list<Event>::iterator it;
        for (it = events.begin(); it != events.end(); ++it) {
            if (e.type <= it->type) {
                break;
            }
        }
        events.insert(it, e);
    }
}

// Añade un listener al mapa
void EventManager::SubscribeMulti(const Listener& listener) {
    //Vamos a ver si tiene ya alguno de este tipo
    auto iterator = listeners.find(listener.type);

    //Si entra es que no habia uno ya creado de ese tipo
    if (iterator == listeners.end()) {
        ListenerVector listenerVector;
        iterator = listeners.insert(std::pair<EventType, ListenerVector>(listener.type, listenerVector)).first;
    }

    iterator->second.push_back(listener);
    //std::cout << "Añadido el Listener: " << listener.name << "\n";
}

// Añade un listener al mapa
void EventManager::Subscribe(const Listener& listener) {
    SubscribeMulti(listener);
}

//TODO: Esto hay que mejorarlo muchisimo pero no controlo aun demasiado tema de punteros e iterators
void EventManager::UnSubscribeMulti(EventType eType, const string& listenerName) {
    ShowSuscribers();
    auto mapByType = listeners.find(eType);

    if (mapByType != listeners.end()) {
        auto listenerVector = mapByType->second;  // El vector de listeners del mapa segun el EventType

        for (uint32_t i = 0; i < listenerVector.size(); ++i) {
            if (listenerVector.at(i).name.compare(listenerName) == 0) {
                //cout << "Borramos el Listener de tipo: " << listenerVector.at(i).type << " con nombre " << listenerVector.at(i).name << endl;
                listenerVector.erase(listenerVector.begin() + i);
                break;
            }
        }

        //TODO: Esto es mucho gasto el borrar todo el vector de listeners y volver a introducirlo
        listeners.erase(mapByType);
        listeners.insert(pair<EventType, ListenerVector>(eType, listenerVector)).first;
    }
    ShowSuscribers();
}

void EventManager::ClearEvents() {
    events.clear();
}

void EventManager::ClearListeners() {
    listeners.clear();
}

//DEBUG: Recorre y muestra todos los suscriptores
//Fuente: https://stackoverflow.com/questions/26281979/c-loop-through-map
void EventManager::ShowSuscribers() {
//    for (auto const& mapByType : listeners) {
//        std::cout << "Tipo de evento: " << mapByType.first << "\n";
//
//        for (auto const& listener : mapByType.second) {
//            cout << listener.name << " - ";
//        }
//        cout << "\n";
//    }
}