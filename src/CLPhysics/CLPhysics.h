/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Carlos de la Fuente Torres <delafuentetorresc@gmail.com>
 * @author Jose Valdés Sirvent
 * 
 */
 
 
 #pragma once

#include <memory>
#include <vector>
#include "../Aliases.h"
#include <glm/vec3.hpp>

using namespace std;

class Manager;
class Entity;
class CTransformable;
class CBoundingSphere;
class CBoundingPlane;
class CBoundingChassis;
class CBoundingOBB;
class CExternalForce;
class ManCar;
class ManPowerUp;
class ManNavMesh;
class ManTotem;
class ManBoxPowerUp;
class ManBoundingOBB;
class ManBoundingWall;
class Camera;
class IntersectData;
class CCar;
class PowerUp;
class Car;
class SystemOnline;


class CLPhysics {
   public:
    CLPhysics();
    ~CLPhysics(){};

    void AddManager(Manager &e);
    void Simulate(float delta);
    void Update(float delta);
    void RepositionBounding();
    void CentralSystemCollisions();
    void CentralSystemGravity();
    void SubscribeToEvents();

    IntersectData HandleCollisionsRayWithSpheres(CTransformable &trCar1, CTransformable &trCar2, CBoundingSphere &spCar2, const glm::vec3 &normalRay);
    IntersectData HandleCollisionsRayWithPlane(CTransformable &trRayOrigin,  glm::vec3 &rayNormalNormalized, CBoundingPlane &planeObject);
    IntersectData HandleCollisionsRayWithOBB(CTransformable &trRayOrigin,  glm::vec3 &rayNormalNormalized, CBoundingOBB &OBBObject);
    // void Integrate(Entity &e, float delta);
    void IntersectsCarsPowerUps(ManCar &, ManPowerUp &, ManNavMesh *);
    void IntersectCarsTotem(ManCar &, ManTotem &);
    void IntersectCarsBoxPowerUp(ManCar &, ManBoxPowerUp &);
    void IntersectPowerUpWalls(ManPowerUp &, ManBoundingWall &, ManBoundingOBB &);
    void IntersectTotemWalls(ManTotem &, ManBoundingWall &, ManBoundingOBB &);
    void IntersectCameraWalls(Camera *cam, Car* car, ManBoundingWall &sd, ManBoundingOBB &sdww);
    void SetSystemOnline(SystemOnline *systemOnline_) {
        systemOnline = systemOnline_;
    };

    static void RunTests();

   protected:
   private:
    // gravity
    void HandleCollisionsWithGround();
    void ConstGravity();
    void aplicateGravity();
    bool CollisionsChassisGround(CTransformable &trCar, CBoundingChassis &chaCar, CCar &ccar, bool mainCar, CBoundingPlane &plane);
    bool CollisionsSphereGround(CTransformable &trCar, CBoundingSphere &spCar, CBoundingPlane &plane); 
    void SeparateSphereGround(IntersectData &intersData, CTransformable &trCar, CBoundingSphere &spCar, CBoundingPlane &plane) const;
    void RePositionCarY(CTransformable &trCar, CBoundingSphere &sp1Car, CBoundingSphere &sp2Car) const;
    void RePositionEntityY(CTransformable &trEntity, CBoundingSphere &sphere) const;
    void RePositionPUY(CTransformable &trEntity, CBoundingSphere &sphere) const;
    void RotateCarXZ(CTransformable &trCar, CBoundingChassis &chaCar, CBoundingPlane *pl1Car, CBoundingPlane *pl2Car) const;
    void LimitRotationCarY() const;
    // collisions
    void HandleCollisions();
    void HandleCollisionsWithPlanes();
    void HandleCollisionsWithOBB();
    bool HandleCollisions(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2);
    bool HandleCollisions(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccar1, bool mainCar, CBoundingPlane &plane);
    bool HandleCollisions(CTransformable &trCar, CBoundingSphere &spCar, CCar &ccarCar, bool mainCar, CBoundingOBB &obb);
    void HandleCollisionPUWithCar(PowerUp *powerUp, Entity *car);
    bool CollisionsBehindBehind(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2);
    bool CollisionsBehindFront(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2);
    bool CollisionsFrontBehind(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2);
    bool CollisionsFrontFront(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2);
    bool CollisionsSphereSphere(CTransformable &trCar1, CBoundingChassis &chaCar1, CCar &ccar1, bool mainCar, CTransformable &trCar2, CBoundingChassis &chaCar2, CCar &ccar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2);
    bool CollisionsCilindreSphere(CTransformable &trCar1, CCar &ccar1, CBoundingChassis &cChaCar1, CTransformable &trCar2, CCar &ccar2, CBoundingChassis &cChaCar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2);
    bool CollisionsSpherePlane(CTransformable &trCar1, CBoundingChassis &chaCar, CCar &ccar1, bool mainCar, CBoundingPlane &plane);
    void CollisionsSphereOBB(CTransformable &trCar, CBoundingChassis &chaCar, CCar &ccarCar, bool mainCar, CBoundingOBB &obb);
    void PositionSphereIntoTransformable(CTransformable &tr, CBoundingSphere &sp) const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void PositionSphBehindIntoTransf(CTransformable &tr, CBoundingSphere &sp) const;
    void PositionSphFrontIntoTransf(CTransformable &tr, CBoundingSphere &sp) const;
    void PositionCilindreIntoSpheres(CBoundingChassis &chassis) const;
    void SeparateSpheres(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2) const;
    void SeparateCilindreSphere(CTransformable &trCar1, glm::vec3 &cenCar1, const float &radCar1, CCar &ccarCar1, CTransformable &trCar2, glm::vec3 &cenCar2, const float &radCar2, CCar &ccarCar2) const;
    void SeparateSphereFromPlane(IntersectData &intersData, CTransformable &trCar1, CBoundingSphere &spCar1, CBoundingPlane &plane) const;
    void ReflectCollision(CTransformable &trCar1, CCar &cCar1, CTransformable &trCar2, CCar &cCar2);
    void ReflectSpherePlane(CTransformable &trCar, CCar &cCar1, CBoundingPlane &plane);
    void ExchangeVectors(CTransformable &trCar1, CCar &cCar1, CTransformable &trCar2, CCar &cCar2);
    void checkCollisionNitro(Entity* car1, Entity* car2);
    void impactCarCar(CCar &ccar1, CCar &ccar2) const;
    void ReciveExternalForce(CExternalForce &cExtForc1, CTransformable &trCar2,  CCar &ccar2);
    double Angle2Vectors(const glm::vec3 &a, const glm::vec3 &b) const;
    glm::vec3 CalculateProyectPointRecta(const glm::vec3 &extrem1, const glm::vec3 &extrem2, const glm::vec3 &point_) const;
    glm::vec3 CalculateVecDirCar(CTransformable &cTransformable) const;

    void NewCrashPUCarReceived(DataMap *d);
    void NewCrashPUWallReceived(DataMap *d);


    vector<Manager *> managers;
    const float gravityCar = -2.0f;
    const float gravityPU = -1.0f;

    SystemOnline *systemOnline {nullptr};
};
