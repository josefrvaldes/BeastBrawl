#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <memory>
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
    vector<shared_ptr<CarAI>> GetEntities() const {
        return CarAIs;
    };

private:
	vector<shared_ptr<CarAI>> CarAIs;
    void SubscribeToEvents();
};