#ifndef __HERO_H__
#define __HERO_H__

#include "../components/position.h"
#include "entity.h"
#include <iostream>
#include <map>

using namespace std;

class Hero : Entity
{

public:
    Hero();
    ~Hero();
};

#endif