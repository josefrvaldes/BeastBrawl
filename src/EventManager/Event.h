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
    PRESS_SKID,
    PRESS_SPACE,
    INVERT_CAMERA,
    TOTEM_CAMERA,
    NORMAL_CAMERA,
    COLLISION_ENTITY_POWERUP,
    COLLISION_CAR_POWERUP,
    DELETE_POWERUP,
    NO_I_O_PRESS,
    NO_A_D_PRESS,
    //ACTUALIZE_NAVMESH_TOTEM,
    //ACTUALIZE_NAVMESH_CARAI,
    NOT_SKID_PRESS,
    MENU_OPTION,
    MENU_OK,
    MENU_BACK,
    ACTUALIZE_NAVMESH_TOTEM,
    ACTUALIZE_NAVMESH_CARAI,
    CHANGE_TOTEM_CAR,
    INIT_PARTICLES_BOX,
    CHANGE_PARTICLES_VISIBILITY,
    CREATE_PARTICLES_COLLISION_POWERUP,
    CREATE_PARTICLES_ROBOJOROBO,
    COLLISION_PLAYER_TOTEM,
    COLLISION_AI_TOTEM,
    PowerUp_Create,
    CATCH_BOX_POWERUP,
    CATCH_AI_BOX_POWERUP,
    CATCH_BOX_WITH_POWERUP,
    DROP_TOTEM,
    THROW_POWERUP,
    THROW_POWERUP_AI,
    THROW_POWERUP_HUMAN,
    MOVE_RANDOM_POWERUP,
    CHANGE_DESTINATION,
    PRESS_P,                    // Sonido pito
    UPDATE_POWERUP_HUD,         // Actualiza el hud de powerup
    SET_EVENT_HUD,
    SET_GLOBAL_VOLUME,          // Actualizar volumen global
    SET_MUSIC_VOLUME,
    SET_RESOLUTION,
    ENABLE_PARTICLES,
    ENABLE_VEGETATION,
    ENABLE_SHADOWS,
    START_MINGAME,
    STOP_MINGAME,
    HURT,                       // Sonido danyo
    CATCH_TOTEM,                // Sonido coger totem
    NO_SHIELD,                  // Sonido escudo roto
    CRASH,
    VICTORY,
    DEFEAT,
    CLOCK,                      // Poner reloj global a sonar
    NOT_CLOCK,                  // Parar reloj global
    STOP_SOUND_MM,              // Parar sonido melon molon
    STOP_SOUND_TB,              // Parar sonido telebanana
    CRASH_WALL,                 // Sonido golpe con pared
    BREAK_BOX,                  // Sonido romper caja
    DRIFT,
    DRIFTS,
    NO_DRIFT,
    VRANDOM,                    // Que el personaje hable en la seleccion
    UPDATE_SOUNDCHARACTER,      // Updatear el personaje que habla
    STATE_INTRO,                //Cambiar al estado Intro
    STATE_MENU,
    STATE_SELECT_CHARACTER,
    STATE_PAUSE,
    STATE_ENDRACE,
    STATE_ENDTOURNAMENT,
    STATE_INGAMESINGLE,
    STATE_INGAMEMULTI,
    STATE_LOBBYMULTI,
    STATE_CONTROLS,
    STATE_CREDITS,
    STATE_GAME_OPTIONS,
    STATE_TOURNAMENT_OPTIONS,
    STATE_SETTINGS,
    LAUNCH_ANIMATION_END_MULTI,
    CALCULATE_PATH_TO_NAVMESH,
    UPDATE_SHIELD_VISIBILITY,
    UPDATE_FACADE_VISIBILITY,
    PREPARE_TO_DISCONNECT,
    PREPARE_TO_SELECT_CHAR,
    NEW_TCP_START_MULTI,
    NEW_INPUTS_RECEIVED,
    NEW_SYNC_RECEIVED_CAR,
    NEW_SYNC_RECEIVED_TOTEM,
    NEW_CATCH_PU_RECEIVED,
    NEW_CATCH_TOTEM_RECEIVED,
    NEW_LOST_TOTEM_RECEIVED,
    NEW_ROBOJOROBO_RECEIVED,
    NEW_NITRO_RECEIVED,
    NEW_THROW_PU_RECEIVED,
    NEW_CRASH_PU_CAR_RECEIVED,
    NEW_CRASH_PU_WALL_RECEIVED,
    NEW_LAUNCH_ANIMATION_END_RECEIVED,
    NEW_LAUNCH_COUNTDOWN_ANIMATION_RECEIVED,
    NEW_CLOCK_SYNC_RECEIVED,
    NEW_FINAL_CLOCK_SYNC_RECEIVED,
    DISCONNECTED_PLAYER,
    TCP_CHAR_REQUEST,
    TCP_WAIT_OTHERS,
    TCP_SEL_CHAR,
    RETURN_TO_SELCHAR
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
    bool stole;
    int num;
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
