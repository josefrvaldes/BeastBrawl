#pragma once
#include <cstdint>
#include <vector>
#include <string>

using namespace std;

//El orden de los enums define la prioridad del evento
enum EventType{
    PRESS_W,
    PRIORIDAD1,
    PRIORIDAD2,
    PRIORIDAD3

};

struct Data{
    uint16_t id;
};

struct Event {

    EventType type;
    Data data;

};

struct Listener{
    EventType type;
    void (*function)(Data); // Puntero a funcion 
    string name;            // Nombre del listener
};

typedef std::vector<Listener> ListenerVector; // Se usará para poder tener varios listener del mismo EventType en el hash map