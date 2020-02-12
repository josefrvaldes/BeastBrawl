#pragma once
#include <stdarg.h>
#include <any>
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "Lambda.h"
#include <Aliases.h>
#include <Entities/Entity.h>
#include <Components/CTransformable.h>
#include <Components/CPowerUp.h>

using namespace std;


//El orden de los enums define la prioridad del evento
enum EventType {
    PRESS_I,
    PRESS_O,
    PRESS_A,
    PRESS_D,
    PRESS_SPACE,
    INVERT_CAMERA,
    TOTEM_CAMERA,
    NORMAL_CAMERA,
    COLLISION_ENTITY_POWERUP,
    COLLISION_ENTITY_AI_POWERUP,
    NO_I_O_PRESS,
    NO_A_D_PRESS,
    ACTUALIZE_NAVMESH_TOTEM,
    ACTUALIZE_NAVMESH_CARAI,
    CHANGE_TOTEM_CAR,
    PRIORIDAD1,
    PRIORIDAD2,
    PRIORIDAD3,
    COLLISION_PLAYER_TOTEM,
    COLLISION_AI_TOTEM,
    POWER_UP_COGIDO,
    PowerUp_Create,
    CATCH_BOX_POWERUP,
    CATCH_AI_BOX_POWERUP,
    CATCH_BOX_WITH_POWERUP,
    DROP_TOTEM,
    THROW_POWERUP,
    THROW_POWERUP_AI,
    THROW_POWERUP_HUMAN,
    MOVE_TO_POWERUP,
    CHANGE_DESTINATION,
    PRESS_P,
    UPDATE_POWERUP_HUD,
    PRESS_0,
    HURT,
    CATCH_TOTEM,
    NO_SHIELD,
    CRASH_ENEMY,
    BREAK_BOX,
    DRIFT,
    NO_DRIFT,
    VRANDOM,
    MENU_OPTION,
    MENU_OK,
    MENU_BACK,
    START_GAME,
    STATE_MENU,
    STATE_PAUSE,
    STATE_ENDRACE,
    STATE_INGAMESINGLE,
    STATE_INGAMEMULTI,
    STATE_LOBBYMULTI,
    CALCULATE_PATH_TO_NAVMESH,
    NEW_TCP_START_MULTI,
    NEW_INPUTS_RECEIVED,
    NEW_SYNC_RECEIVED_CAR,
    NEW_SYNC_RECEIVED_TOTEM
};

// enum DataType{
//     TYPE_POWER_UP,
//     CAR_WITH_TOTEM,
//     CAR_WITHOUT_TOTEM,
//     MAIN_CAR,
//     CAR_AI,
//     BOX_POSITION,
//     ACTUAL_CAR,
//     TOTEM,
//     CAR_FOLLOW_POSITION,
//     CAR_EXIT_POSITION,
//     CAR_EXIT_DIMENSION,
//     BOX_POWER_UP_COMPONENT,
//     ACTUAL_BOX,
//     POWER_UP,
//     CAR_TRANSFORMABLE,
//     MAN_NAVMESH,
//     MAN_WAYPOINTS,
//     POS_DESTINATION
// };

struct Data {
    uint16_t id;
    Entity* gameObject;
    Entity* camera;
    float deltaTime;
    CTransformable* posCocheSalida;
    CTransformable* posCochePerseguir;
    typeCPowerUp typePowerUp;
    bool mainCharacter;
};

struct Event {
    EventType type;
    shared_ptr<DataMap> data;
    Event(EventType _type) : type{_type} {
    }
    Event(EventType _type, shared_ptr<DataMap> _data) : type{_type}, data{_data} {
    }
};

struct Listener {
    uint32_t id = 0;
    EventType type;
    // void (*callback)(Data);  // Puntero a funcion
    // TransientFunction<void(Data)> callback;  // Puntero a funcion
    // template<typename T> class Lambda {};
    // template<typename Out, typename... In> class Lambda<Out(In...)> {};
    // Lambda<void(Data d)> callback;
    // template <typename Args>
    // Lambda<void(int)> callback;


    function<void(DataMap*)> callback;
    string name;  // Nombre del listener

    // con transient
    // Listener(EventType _type, TransientFunction<void(Data)> _callback, string _name)
    //     : type(_type), callback(_callback), name(_name) {
    // }

    // con std::function
    Listener(EventType _type, function<void(DataMap*)> _callback, string _name)
        : type(_type), callback(_callback), name(_name) {
    }

    // con Lambda
    // Listener(EventType _type, Lambda<void(int)> _callback, string _name)
    //     : type(_type), callback(_callback), name(_name) {
    // }

    // con puntero a función
    // Listener(EventType _type, void (*_callback)(Data), string _name)
    //     : type(_type), callback(_callback), name(_name) {
    // }

    // descomentar cuando haga falta el id del listener
    /*Listener(uint32_t _id, EventType _type, function<void(Data)> _callback, string _name)
        : id(_id), type(_type), callback(_callback), name(_name) {
    }*/
};

typedef std::vector<Listener> ListenerVector;  // Se usará para poder tener varios listener del mismo EventType en el hash map
