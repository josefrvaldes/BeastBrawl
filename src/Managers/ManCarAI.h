#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "../../lib/glm/vec3.hpp"

using namespace std;

class CarAI;

class ManCarAI
{

public:
    ManCarAI();
    ~ManCarAI();

    void CreateCarAI(glm::vec3 _position);
    void CreateCarAI();
    vector<CarAI *> GetEntities() const {
        return CarAIs;
    };

private:
	vector<CarAI *> CarAIs;
    void SubscribeToEvents();
};