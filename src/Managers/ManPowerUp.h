#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "../../lib/glm/vec3.hpp"

using namespace std;

class PowerUp;

class ManPowerUp
{

public:
    ManPowerUp();
    ~ManPowerUp();

    void CreatePowerUp(glm::vec3 _position);
    void CreatePowerUp();
    vector<PowerUp *> GetEntities() const {
        return powerUps;
    };

private:
	vector<PowerUp *> powerUps;
    void SubscribeToEvents();
};