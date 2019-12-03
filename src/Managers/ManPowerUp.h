#pragma once

#include <iostream>
#include <map>
#include <vector>

using namespace std;

class PowerUp;

class ManPowerUp
{

public:
    ManPowerUp();
    ~ManPowerUp();

    void CreatePowerUp(float x, float y, float z);
    void CreatePowerUp();
    vector<PowerUp *> GetEntities() const {
        return powerUps;
    };

private:
	vector<PowerUp *> powerUps;
    void SubscribeToEvents();
};