#pragma once

#include "SystemAI.h"
#include <glm/vec3.hpp>
#include <memory>
#include <CLPhysics/CLPhysics.h>
#include "../Systems/SystemVision.h"


using namespace std;

struct Manager;
struct CarAI;


class SystemVisionAI : public SystemAI {
public:
    SystemVisionAI();
    ~SystemVisionAI(){};

    void update(CarAI* actualCar) override;
    void AddManager(Manager &e);
    int getFrecuency(){ return frec; };

private:

    vector<Manager *> managers;
    int frec {1};

    unique_ptr<CLPhysics> clPhysics;
    unique_ptr<SystemVision> systemVision;
};