#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "SystemAI.h"

using namespace std;

struct selector;
struct Manager;
struct CarAI;

class SystemBtPowerUp : public SystemAI{
   public:
    SystemBtPowerUp();
    ~SystemBtPowerUp(){};

    void init();
    void update(CarAI* actualCar) override;

    void AddManager(Manager &e);


    int getFrecuency(){ return frec; };
    
   protected:
   private:
   shared_ptr<selector> selectorBehaviourTree;

   vector<Manager *> managers;

   int frec {6};

};