#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include "../Systems/SystemVision.h"
#include "../Entities/Car.h"


using namespace std;

struct Manager;
struct Entity;


class SystemData{
public:
    SystemData();
    ~SystemData(){};

    void AddManager(Manager &e);
    Car* CalculateCenteredCar(Entity* actualCar, int visionRange) const;
    bool EntityInVisionHumanSD(const glm::vec3& posEntity) const{ return systemVision->EntityInVisionHuman(posEntity); };

private:
    vector<Manager *> managers;
    unique_ptr<SystemVision> systemVision;
};