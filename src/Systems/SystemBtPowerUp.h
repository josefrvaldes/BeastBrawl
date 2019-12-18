#pragma once

#include "../behaviourTree/behaviourTree.h"
#include "../behaviourTree/composite.h"
#include "../behaviourTree/selector.h"
#include "../behaviourTree/sequence.h"
#include "../behaviourTree/decorator.h"

#include <iostream>

using namespace std;


class SystemBtPowerUp {
   public:
    SystemBtPowerUp();
    ~SystemBtPowerUp(){};

    void init();
    void update();


   protected:
   private:
   shared_ptr<selector> selectorBehaviourTree;

};