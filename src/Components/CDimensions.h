#pragma once

#include "Component.h"

#include <string>
#include <iostream>

using namespace std;

class CDimensions : public Component
{
public:
    CDimensions();
    CDimensions(float _width,float _height, float _depth);
    ~CDimensions(){};

    float width;
    float height;
    float depth;

};