#pragma once



#include <iostream>
#include <memory>
#include <vector>

#include "SystemAI.h" 

using namespace std;

struct selector;
struct CarAI;
struct Manager;
struct ManCar;
struct SystemFuzzyLogicAI;
struct SteeringBehaviours;
struct CLPhysics;

class SystemBtLoDMove : public SystemAI{
   public:
      SystemBtLoDMove();
      ~SystemBtLoDMove(){};

      void init();
      void update(CarAI* actualCar) override;

      void AddManager(Manager &);
      void AddCLPhysicsSB(CLPhysics* clPhysics);
      void InitFuzzyLogic(ManCar &);

      int getFrecuency(){ return frec; };


   private:
      shared_ptr<selector> selectorBehaviourTree;
      //shared_ptr<SystemFuzzyLogicAI> fuzzyLogic;
      shared_ptr<SteeringBehaviours> steeringBehaviours;

      vector<Manager *> managers;
      //bool entradoFL = false;

      int frec {1};

};