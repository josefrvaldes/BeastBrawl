#ifndef __MAN_POWERUP_H__
#define __MAN_POWERUP_H__

#include "../entities/PowerUp.h"
#include <iostream>
#include <map>

using namespace std;

class ManPowerUp
{

public:
    ManPowerUp();
    ~ManPowerUp();

private:
	vector<PowerUp> powerUps;
};

#endif