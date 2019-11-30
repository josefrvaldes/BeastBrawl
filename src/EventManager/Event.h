#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "../Entities/Entity.h"

using namespace std;

//El orden de los enums define la prioridad del evento
enum EventType{
    PRESS_W,
    PRESS_S,
    PRESS_A,
    PRESS_D,
    NO_W_S_PRESS,
    NO_A_D_PRESS,
    PRIORIDAD1,
    PRIORIDAD2,
    PRIORIDAD3

};

struct Data{
    uint16_t id;
    Entity* gameObject;
    Entity* camera;
    float deltaTime;
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