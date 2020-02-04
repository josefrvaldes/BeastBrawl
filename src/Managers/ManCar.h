#pragma once


#include "../../include/glm/vec3.hpp"
#include "../Aliases.h"
#include "../Components/CWayPoint.h"
#include "../Components/CTransformable.h"
#include "../Components/CSpeed.h"
#include "Manager.h"

#include "../Managers/ManWayPoint.h"
#include "../Entities/Entity.h"
#include "../Entities/WayPoint.h"

#include "../Systems/SystemBtPowerUp.h"
#include "../Systems/SystemBtMoveTo.h"
#include "../Systems/SystemBtLoDMove.h"
#include "../Systems/SystemPathPlanning.h"

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

class ManCar : public Manager {
   public:
    ManCar();
    ManCar(Physics *, Camera *);
    ~ManCar();

    void CreateMainCar();
    void CreateHumanCar(glm::vec3 _position);
    void UpdateCar();
    void UpdateCarAI(CarAI* carAI, ManPowerUp* m_manPowerUp, ManBoxPowerUp* m_manBoxPowerUp, ManTotem* m_manTotem, ManWayPoint* graph, ManNavMesh* manNavMesh, ManBoundingWall* m_manBoundingWall);
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
    //void ChangePosDestination(DataMap* d);
    //void MoveToPowerUp(DataMap* d);
    void ThrowTotem(Entity* carLoseTotem);
    bool useRoboJorobo(Entity* newCarWithTotem);

    void ThrowPowerUp(DataMap* d);
    void ThrowPowerUpAI(DataMap* d);
    void CatchPowerUp(DataMap* d);
    void CatchPowerUpAI(DataMap* d);
    void InitMapGraph(ManWayPoint* _graph);
    float** graph;
    int graphSize = 0;
    bool graphCreated = false;
    shared_ptr<CarHuman> car;
 
    unique_ptr<SystemBtPowerUp> systemBtPowerUp;
    unique_ptr<SystemBtMoveTo> systemBtMoveTo;
    unique_ptr<SystemBtLoDMove> systemBtLoDMove;
    unique_ptr<PhysicsAI> physicsAI;
    unique_ptr<SystemPathPlanning> systemPathPlanning;
};
