#pragma once

#include <memory>
#include <vector>

using namespace std;

class Manager;

class CLPhysics {
   public:
    CLPhysics();
    ~CLPhysics(){};

    void AddManager(Manager &e);
    void Simulate(float delta);
    void Update(float delta);
    void HandleCollisions();

    // void Integrate(Entity &e, float delta);

    static void RunTests();

   protected:
   private:
    vector<Manager *> managers;
};