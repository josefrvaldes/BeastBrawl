#pragma once

#include "Entity.h"
#include <string>
#include <iostream>
#include <glm/vec3.hpp>
#include <Components/CPosDestination.h>
#include <Components/CWayPoint.h>

using namespace std;

enum class TypeCar { CarHuman, CarAI };

class Car : public Entity {
    public:
    
        Car(int pj);
        ~Car(){};

        TypeCar GetTypeCar(){ return typeCar; };

    protected:
    
        TypeCar typeCar;
    
};
