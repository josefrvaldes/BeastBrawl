#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Manager.h"

using namespace std;
class Car;
class Data;

class ManCar : public Manager {
   public:
    ManCar();
    ~ManCar();

    void CreateCar();
    void CreateMainCar();
    shared_ptr<Car>& GetCar() {
        return car;
    };

   private:
    void SubscribeToEvents();
    void AcelerarCoche(Data d);
    shared_ptr<Car> car;
};
