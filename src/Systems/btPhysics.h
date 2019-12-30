#pragma once

#include "btBulletDynamicsCommon.h"

#include <memory>

using namespace std;
#define PI 3.141592

class Car;
class Camera;

class btPhysics {
   public:
    btPhysics(float *);
    ~btPhysics(){};
    void update(Car* car, Camera* cam);

   protected:
   private:
    float *deltaTime;

    unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	unique_ptr<btCollisionDispatcher> dispatcher;

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	unique_ptr<btBroadphaseInterface> overlappingPairCache;

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	unique_ptr<btSequentialImpulseConstraintSolver> solver;
    unique_ptr<btDynamicsWorld> dynamicsWorld; // hace falta un mundo virtual donde se simularán todas las físicas
};