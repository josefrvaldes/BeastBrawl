#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "Entity.h"
#include <iostream>
#include <map>

using namespace std;

class PowerUp : public Entity
{

public:
    PowerUp();
    PowerUp(float x, float y, float z);
    ~PowerUp();


private:
};

#endif