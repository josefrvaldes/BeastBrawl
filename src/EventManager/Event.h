#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include "../Entities/Entity.h"
#include "../Components/CTransformable.h"
#include "../Components/CPowerUp.h"

using namespace std;

//El orden de los enums define la prioridad del evento
enum EventType {
    PRESS_I,
    PRESS_O,
    PRESS_A,
    PRESS_D,
    PRESS_SPACE,
    NO_I_O_PRESS,
    NO_A_D_PRESS,
    PRIORIDAD1,
    PRIORIDAD2,
    PRIORIDAD3,
    POWER_UP_COGIDO,
    PowerUp_Create,
    PRESS_C
};

struct Data {
    uint16_t id;
    Entity* gameObject;
    Entity* camera;
    float deltaTime;
    CTransformable* posCocheSalida;
    CTransformable* posCochePerseguir;
    typeCPowerUp typePowerUp;
};

struct Event {
    EventType type;
    Data data;
};

struct Listener {
    uint32_t id = 0;
    EventType type;
    //void (*function)(Data);  // Puntero a funcion
    function<void(Data)> callback;
    string name;             // Nombre del listener
    
    Listener(EventType _type, function<void(Data)> _function, string _name) 
     : type(_type), callback(_function), name(_name)
    {
        
    }

    Listener(uint32_t _id, EventType _type, function<void(Data)> _callback, string _name) 
     : id(_id), type(_type), callback(_callback), name(_name)
    {
        
    }    
};

typedef std::vector<Listener> ListenerVector;  // Se usará para poder tener varios listener del mismo EventType en el hash map