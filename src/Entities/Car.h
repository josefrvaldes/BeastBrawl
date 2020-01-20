#pragma once



#include "Entity.h"
using namespace std;
enum class TypeCar { CarHuman, CarAI };
class Car : public Entity {
   public:
    Car(){};
    ~Car(){};

    TypeCar GetTypeCar(){ return typeCar; };

    protected:
    TypeCar typeCar;
    
};
