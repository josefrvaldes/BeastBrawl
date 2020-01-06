#pragma once

#include <memory>
#include <vector>

using namespace std;

class Manager;
class CTransformable;
class CBoundingSphere;
class CCar;
class CColliding;

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
    void HandleCollisions(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &, CColliding &, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &, CColliding &);
    void RestartCollisionTimeIfNeeded(CColliding &collidingCar);
    void PositionSphereIntoTransformable(CTransformable &tr, CBoundingSphere &sp);
    void SeparateSpheres(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, CColliding &colliding1,
                                 CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2, CColliding &colliding2);
    void ReflectCollision(CTransformable &trCar1, CCar &cCar1, CTransformable &trCar2, CCar &cCar2);
    void ExchangeVectors(CTransformable &trCar1, CCar &cCar1, CTransformable &trCar2, CCar &cCar2);
    vector<Manager *> managers;
};