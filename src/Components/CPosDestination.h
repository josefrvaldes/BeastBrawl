#pragma once

#include "Component.h"

#include <glm/vec3.hpp>
#include <string>
#include <iostream>

using namespace std;
//using namespace glm;

class CPosDestination : public Component
{
public:
    CPosDestination();
    CPosDestination(glm::vec3 _pos, int _type,int _id);
    CPosDestination(glm::vec3 _pos, int _type,int _id, float _radious);
    ~CPosDestination();

    glm::vec3 position;
    int id;
    float radious;
    //int type;
    // tenemos varios tipos:
    // 0 : cuando el destino es un nodo
    // 1 : cuando el destino es un Totem o un coche 
};