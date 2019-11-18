#include "EventManager.h"
#include <iostream>

EventManager& EventManager::GetInstance() {
    static EventManager instance;
    return instance;
}


//Realiza y vacia todos los eventos que estaban añadidos
//O(n)
//Acceso: O(1)
void EventManager::Update(){

/** Version con cola 
    while(!eventQueue.empty()){
        Event e = eventQueue.front(); //Cojemos el primero en la lista
        eventQueue.pop(); // Lo sacamos de la lista

        cout << "Procesando evento con prioridad: " << e.type << endl;

        //Tratamos el evento
        auto mapByType = eventListenerMap.find(e.type);

        if(mapByType!=eventListenerMap.end()){
            auto eventVector = mapByType->second;   // El vector de listeners del mapa segun el EventType

            for(Listener listener : eventVector){
                listener.function(e.data);
            }
        }
   }
*/    
   while(!eventList.empty()){
        Event e = eventList.front(); //Cojemos el primero en la lista
        eventList.pop_front(); // Lo sacamos de la lista

        cout << "Procesando evento con prioridad: " << e.type << endl;

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
// O(n) -> Recorremos la lista iterativamente hasta encontrar el valor directamente superior
void EventManager::AddEvent(Event e){
    //FIXME: Descomentar esto para que funcione con cola
    //eventQueue.push(e);

    list <Event> :: iterator it; 

    //Si es el primer evento lo añadimos al comienzo
    if(eventList.size() == 0){
        eventList.push_front(e);
    }else{
        //TODO: Implementar un algoritmo de busqueda e insercion mas optimo
        //Movemos el iterator hasta posicionarlo antes del siguiente valor mas grande
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
        iterator = eventListenerMap.insert(pair<EventType,ListenerVector>(listener.type, listenerVector)).first;
    }

    iterator->second.push_back(listener);



}