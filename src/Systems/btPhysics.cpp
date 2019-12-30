#include "btPhysics.h"

btPhysics::btPhysics(float* delta) : deltaTime{delta} {
    // collisionConfiguration = make_unique<btDefaultCollisionConfiguration>();

    // ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    // dispatcher = make_unique<btCollisionDispatcher>(collisionConfiguration);

    // ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    // overlappingPairCache = make_unique<btDbvtBroadphase>();

    // ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    // solver = make_unique<btSequentialImpulseConstraintSolver>();
    // dynamicsWorld = make_unique<btDiscreteDynamicsWorld>(dispatcher, overlappingPairCache, solver, collisionConfiguration);
}