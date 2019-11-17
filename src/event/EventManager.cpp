#include "EventManager.h"
#include <iostream>

EventManager& EventManager::GetInstance() {
    static EventManager instance;
    return instance;
}



void EventManager::Update(){
    
   while(!eventList.empty()){
        Event e = eventList.front(); //Cojemos el primero en la lista
        eventList.pop_front(); // Lo sacamos de la lista

        std::cout << "Procesando evento con prioridad: " << e.type << std::endl;

        //Tratamos el evento
        auto mapByType = eventListenerMap.find(e.type);

        if(mapByType!=eventListenerMap.end()){
            auto eventVector = mapByType->second;   // El vector de listeners del mapa segun el EventType

            for(Listener listener : eventVector){
                listener.function(e.data);
            }
        }
   }
}
//Añade un evento a la cola de eventos
void EventManager::AddEvent(Event e){
    eventQueue.push(e);

    std::list <Event> :: iterator it; 

    //Si es el primer evento lo añadimos al comienzo
    if(eventList.size() == 0){
        eventList.push_front(e);
    }else{
        //Recorremos la lista para añadirlo ordenado
        for(it = eventList.begin(); it != eventList.end(); ++it){
            if(e.type <= it->type){
                break;
            }
        }

        eventList.insert(it,e);
    
    }
}


// Añade un listener al mapa
void EventManager::Suscribe(Listener listener){

    //Vamos a ver si tiene ya alguno de este tipo
    auto iterator = eventListenerMap.find(listener.type);

    //Si entra es que no habia uno ya creado de ese tipo
    if(iterator == eventListenerMap.end()){
        ListenerVector listenerVector;
        iterator = eventListenerMap.insert(std::pair<EventType,ListenerVector>(listener.type, listenerVector)).first;
    }

    iterator->second.push_back(listener);



}