#pragma once

#include "Entity.h"
#include <iostream>

using namespace std;

class NamePlate : public Entity
{

public:
    NamePlate(int parentNode, string billboardPath);
    ~NamePlate();
};


