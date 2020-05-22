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
 * @author Clover Games Studio
 * 
 */
 
 
 #pragma once

#include "ManWayPoint.h"

#include <glm/vec3.hpp>
#include <Aliases.h>
#include <Components/CWayPoint.h>
#include <Components/CTransformable.h>
#include <Components/CSpeed.h>
#include <Components/CBrainAI.h>

#include <Entities/Entity.h>
#include <Entities/WayPoint.h>

#include <Systems/SystemPathPlanning.h>
#include "../Systems/SystemOnline.h"
#include "../Systems/SystemData.h"

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
//struct Camera;
struct ManTotem;
//struct PhysicsAI;
struct SystemBtPowerUp;
struct SystemBtMoveTo;
struct SystemBtLoDMove;
struct SystemGameRules;


class ManCar : public Manager {
   public:
    ManCar(std::vector<glm::vec3>);
    //ManCar();
    ~ManCar();

    void CreateMainCar(int pj);
    void CreateMainCar(int pj, glm::vec3 _position);
    void CreateHumanCar(int pj, glm::vec3 _position);
    void AddSystemDataVision(SystemData* sd) {systemDataVision = sd;};

    /**
     * @return true o false dependiendo si la partida debe terminar o no
     */
    bool UpdateCarPlayer(ManTotem &);
    bool UpdateGeneralCar(Entity& car_, Entity& totem_);
    bool UpdateCarAI(CarAI* carAI, ManTotem* m_manTotem);
    bool UpdateCarHuman(Entity* CarHuman, ManTotem*);


    shared_ptr<CarHuman>& GetCar() { return car; };

    void CreateCarAI(int pj, int difficult, float timeTotem);
    void CreateCarAI(int pj, int difficult, float timeTotem, glm::vec3 _position);
    void CreateCarAI(int pj, int difficult, float timeTotem, glm::vec3 _position, CWayPoint* _waypoint);
    void Integrate(float) override;
    void SetSystemOnline(SystemOnline* systOn){ systemOnline = systOn; };
    Entity* GetCurrentWinner();
    
    SystemData* systemDataVision = nullptr; 


    glm::vec3 GetPosSpawn();
    glm::vec3 GetPosSpawn(size_t index);
    float GetAngleToTotem(glm::vec3 posCar);

   private:
    //Physics *physics;
    //Camera *cam;
    void AccelerateCar(DataMap* d);
    void SubscribeToEvents() override;
    void TurnLeftCar(DataMap* d);
    void TurnRightCar(DataMap* d);
    void NotAcceleratingOrDecelerating(DataMap* d);
    void Decelerate(DataMap* d);
    void NotTurning(DataMap* d);
    void SkidCar(DataMap* d);
    void NotSkid(DataMap* d);
    void CollisionCarPowerUp(DataMap* d);
    void CatchTotemCar(DataMap* d);
    void ObtainTotem(Entity* carWinTotem, bool stolen);
    void ChangeTotemCar(DataMap* d);
    void NewInputsReceived(DataMap* d);
    void NewSyncReceived(DataMap* d);
    void NewCatchPUReceived(DataMap* d);
    void NewCatchTotemReceived(DataMap* d);
    void NewLostTotemReceived(DataMap* d);
    void NewUsedRoboJoroboReceived(DataMap* d);
    void NewCollideNitroReceived(DataMap* d);
    void DeletePlayer(DataMap* d);
    //void ChangePosDestination(DataMap* d);
    //void MoveToPowerUp(DataMap* d);
    void ThrowTotem(Entity* carLoseTotem, bool stolen);
    bool useRoboJorobo(Entity* newCarWithTotem);

    void ThrowPowerUp(Car* car);
    void ThrowPowerUpCar(DataMap* d);
    void ThrowPowerUpHuman(DataMap* d);
    void ThrowPowerUpAI(DataMap* d);
    void CatchPowerUp(DataMap* d);
    bool CheckIfPUWillBeFired(Car* car_);
    void CatchPowerUpAI(DataMap* d);


    shared_ptr<CarHuman> car;
    //unique_ptr<PhysicsAI> physicsAI;
    unique_ptr<SystemGameRules> systemGameRules;
    unique_ptr<Physics> physics;

    SystemOnline* systemOnline = nullptr; // en caso de que sea Single va a ser un nullptr

    std::vector<glm::vec3> positionsSpawn;

    double maxTimeAccumulated {0};
};
