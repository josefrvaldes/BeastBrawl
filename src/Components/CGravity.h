#pragma once

#include "Component.h"

#include <iostream>

class CGravity : public Component
{
public:
    CGravity();
    ~CGravity(){};

    float currentGravity;
};