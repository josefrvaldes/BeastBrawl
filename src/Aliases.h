#pragma once

#include <unordered_map>
#include <string>
#include <any>



class EventManager;

enum DataType{
    TYPE_POWER_UP,
    CAR_WITH_TOTEM,
    CAR_WITHOUT_TOTEM,
    MAIN_CAR,
    CAR_AI,
    BOX_POSITION,
    ACTUAL_CAR,
    TOTEM,
    TRUEFALSE,
    CAR_FOLLOW_POSITION,
    CAR_FOLLOW,
    CAR_EXIT_POSITION,
    CAR_EXIT_DIMENSION,
    BOX_POWER_UP_COMPONENT,
    ACTUAL_BOX,
    POWER_UP,
    CAR_TRANSFORMABLE,
    MAN_NAVMESH,
    MAN_WAYPOINTS,
    ROTATION,
    SPEED,
    POS_DESTINATION,
    DATA_SERVER,
    ID,
    ID_CAR,
    ID_WINNER,
    ID_PU,
    ID_DESTINATION,
    INPUTS,
    ID_ONLINE,
    ID_PURSUE,
    VECTOR_ID_ONLINE,
    CHARACTERS_ONLINE,
    VEC3_POS,
    VEC3_ROT,
    TIME_TOTEM,
    STOLE,
    NUM,
    TYPE,
    SCREEN_WIDTH,
    SCREEN_HEIGHT
};

using EM = EventManager;

using DataMap = std::unordered_map<DataType, std::any>;// DataMap;