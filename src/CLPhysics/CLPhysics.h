#pragma once

#include <memory>
#include <vector>

using namespace std;

class Manager;
class CTransformable;
class CBoundingSphere;

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
    void HandleCollisions(CTransformable &trCar1, CBoundingSphere &spCar1, CTransformable &trCar2, CBoundingSphere &spCar2);
    vector<Manager *> managers;
};