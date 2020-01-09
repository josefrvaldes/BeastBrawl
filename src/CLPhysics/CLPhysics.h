#pragma once

#include <memory>
#include <vector>
#include "../../lib/glm/vec3.hpp"

using namespace std;

class Manager;
class CTransformable;
class CBoundingSphere;
class CBoundingPlane;
class IntersectData;
class CCar;


class CLPhysics {
   public:
    CLPhysics();
    ~CLPhysics(){};

    void AddManager(Manager &e);
    void Simulate(float delta);
    void Update(float delta);
    void HandleCollisions();
    void HandleCollisionsWithPlanes();

    IntersectData HandleCollisionsRayWithSpheres(CTransformable &trCar1, CTransformable &trCar2, CBoundingSphere &spCar2, const glm::vec3 &normalRay);

    // void Integrate(Entity &e, float delta);

    static void RunTests();

   protected:
   private:
    void HandleCollisions(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccar2);
    void HandleCollisions(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccar1, bool mainCar, CBoundingPlane &plane);
    void PositionSphereIntoTransformable(CTransformable &tr, CBoundingSphere &sp);
    void SeparateSpheres(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1,
                                 CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2);
    void SeparateSphereFromPlane(IntersectData &intersData, CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, CBoundingPlane &plane) const;
    void ReflectCollision(CTransformable &trCar1, CCar &cCar1, CTransformable &trCar2, CCar &cCar2);
    void ExchangeVectors(CTransformable &trCar1, CCar &cCar1, CTransformable &trCar2, CCar &cCar2);
    void SonarChoque(bool mainCar);
    vector<Manager *> managers;
};