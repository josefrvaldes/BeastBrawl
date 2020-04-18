#pragma once

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

//struct selector;
struct Manager;
struct CarAI;

class SystemAI {
   public:
      SystemAI();
      ~SystemAI(){};

      //virtual void init();
      virtual void update(CarAI* actualCar) = 0;   

      void setMaxProcessTime(double maxTime) { maxProcessTime = maxTime; };
      double getMaxProcessTime()             { return maxProcessTime; };

   protected:
      //private:
      //shared_ptr<selector> selectorBehaviourTree;
      double maxProcessTime {0.0005};

};