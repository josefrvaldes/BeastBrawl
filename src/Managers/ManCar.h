#pragma once

#include "ManWayPoint.h"

#include <glm/vec3.hpp>
#include <Aliases.h>
#include <Components/CWayPoint.h>
#include <Components/CTransformable.h>
#include <Components/CSpeed.h>

#include <Entities/Entity.h>
#include <Entities/WayPoint.h>

#include <Systems/SystemPathPlanning.h>

#include <cstdlib> /* srand, rand */
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <stack>

using namespace std;
struct Car;
struct CarHuman;
struct CarAI;
struct Data;
struct Physics;
struct Camera;
struct ManPowerUp;
struct ManBoxPowerUp;
struct ManTotem;
struct ManNavMesh;
struct ManBoundingWall;
struct PhysicsAI;
struct SystemBtPowerUp;
struct SystemBtMoveTo;
struct SystemBtLoDMove;


class ManCar : public Manager {
   public:
    ManCar();
    ManCar(Physics *, Camera *);
    ~ManCar();

    void CreateMainCar();
    void CreateHumanCar(glm::vec3 _position);
    void UpdateCar();
    void UpdateCarAI(CarAI* carAI, ManPowerUp* m_manPowerUp, ManBoxPowerUp* m_manBoxPowerUp, ManTotem* m_manTotem, ManWayPoint* graph, ManNavMesh* manNavMesh, 
                    ManBoundingWall* m_manBoundingWall, SystemBtPowerUp* systemBtPowerUp, SystemBtMoveTo* systemBtMoveTo, SystemBtLoDMove* systemBtLoDMove, SystemPathPlanning *systemPathPlanning);
    void UpdateCarHuman(Entity* CarHuman);
    shared_ptr<CarHuman>& GetCar() { return car; };

    void CreateCarAI();
    void CreateCarAI(glm::vec3 _position);
    void CreateCarAI(glm::vec3 _position, CWayPoint* _waypoint);
    CTransformable* calculateCloserCar(Entity* actualCar);
    bool carInVisionRange(Entity* actualCar, Entity* otherCar, uint32_t rangeVision);
    bool anyCarInVisionRange(Entity* actualCar, uint32_t rangeVision);
    void Integrate(float) override;
    Entity* GetDesirableTarget(Entity* actualCar);
    

   private:
    Physics *physics;
    Camera *cam;
    void AccelerateCar(DataMap* d);
    void SubscribeToEvents() override;
    void TurnLeftCar(DataMap* d);
    void TurnRightCar(DataMap* d);
    void NotAcceleratingOrDecelerating(DataMap* d);
    void Decelerate(DataMap* d);
    void NotTurning(DataMap* d);
    void CollisionPowerUp(DataMap* d);
    void CollisionPowerUpAI(DataMap* d);
    void CatchTotemPlayer(DataMap* d);
    void CatchTotemAI(DataMap* d);
    void UseTotem(Entity* carWinTotem);
    void ChangeTotemCar(DataMap* d);
    void NewInputsReceived(DataMap* d);
    void NewSyncReceived(DataMap* d);
    //void ChangePosDestination(DataMap* d);
    //void MoveToPowerUp(DataMap* d);
    void ThrowTotem(Entity* carLoseTotem);
    bool useRoboJorobo(Entity* newCarWithTotem);

    void ThrowPowerUp(Car* car);
    void ThrowPowerUpCar(DataMap* d);
    void ThrowPowerUpHuman(DataMap* d);
    void ThrowPowerUpAI(DataMap* d);
    void CatchPowerUp(DataMap* d);
    void CatchPowerUpAI(DataMap* d);
    shared_ptr<CarHuman> car;
    unique_ptr<PhysicsAI> physicsAI;
};
