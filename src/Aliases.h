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
    CAR_FOLLOW_POSITION,
    CAR_EXIT_POSITION,
    CAR_EXIT_DIMENSION,
    BOX_POWER_UP_COMPONENT,
    ACTUAL_BOX,
    POWER_UP,
    CAR_TRANSFORMABLE,
    MAN_NAVMESH,
    MAN_WAYPOINTS,
    POS_DESTINATION,
    DATA_SERVER,
    ID,
    INPUTS,
    ID_ONLINE,
    VECTOR_ID_ONLINE,
    VEC3_POS,
    VEC3_ROT,
    TIME_TOTEM
};

using EM = EventManager;

using DataMap = std::unordered_map<DataType, std::any>;// DataMap;