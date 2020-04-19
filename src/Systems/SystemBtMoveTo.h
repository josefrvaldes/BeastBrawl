#pragma once

#include <iostream>
#include <memory>

#include "SystemAI.h"

using namespace std;

struct selector;
struct Manager;
struct CarAI;

class SystemBtMoveTo : public SystemAI{
   public:
    SystemBtMoveTo();
    ~SystemBtMoveTo(){};

    void init();
    void update(CarAI* actualCar) override;

    void AddManager(Manager &e);

    int getFrecuency(){ return frec; };

   protected:
   private:
   shared_ptr<selector> selectorBehaviourTree;

   vector<Manager *> managers;
   int frec {1};
};