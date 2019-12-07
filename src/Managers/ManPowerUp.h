#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <memory>
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
    vector<shared_ptr<PowerUp>> GetEntities() const {
        return powerUps;
    };

private:
	vector<shared_ptr<PowerUp>> powerUps;
    void SubscribeToEvents();
};