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
    IS_ONLINE,
    BOX_POWER_UP_COMPONENT,
    ACTUAL_BOX,
    POWER_UP,
    CAR_TRANSFORMABLE,
    MAN_NAVMESH,
    MAN_WAYPOINTS,
    ROTATION,
    SPEED,
    WHEEL_ROTATION,
    SKID_DEG,
    SKID_ROTATION,
    POS_DESTINATION,
    DATA_SERVER,
    ID,
    ID_CAR,
    ID_WINNER,
    ID_PU,
    ID_DESTINATION,
    INPUTS,
    TIME,
    TURNOUT,
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