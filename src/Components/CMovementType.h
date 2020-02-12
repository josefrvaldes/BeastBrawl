#pragma once

#include "Component.h"

#include <string>
#include <iostream>

class CMovementType : public Component{
public:
    CMovementType(std::string);
    ~CMovementType(){};


    std::string movementType;
};
