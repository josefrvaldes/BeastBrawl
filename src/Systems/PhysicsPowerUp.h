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

#include <Components/CType.h>
#include <Components/CId.h>
#include <Components/CTransformable.h>
#include <Components/CTargetEntity.h>
#include <Components/CCamera.h>
#include <Components/Component.h>
#include <Components/CCar.h>
#include <Components/CWayPoint.h>

#include <Entities/PowerUp.h>
#include <Entities/Entity.h>


// solo lo necesitaran algunos como la banana
#include <Managers/ManWayPoint.h>
#include <Entities/WayPoint.h>

#include <EventManager/EventManager.h>
#include <EventManager/Event.h>

// solo algunos como la banana
#include <fuzzyLogic/fuzzyLogic.h>

#include <vector>
#include <cstdlib>     /* srand, rand */

#include <cmath>
#include <memory>
using namespace std;
#define PI 3.141592

class PhysicsPowerUp
{
public:
    PhysicsPowerUp();
    ~PhysicsPowerUp(){};

    //void init();
    void update(Entity* actualPowerUp);

private:
    void updatePudinDeFrambuesa(Entity* pu);
    void updateTeleBanana(Entity* pu);
    void updateMelonMolon(Entity* pu);
    //uint32_t actualWayPoint = 0;
    //shared_ptr<FuzzyLogic> flVelocity = make_shared<FuzzyLogic>();
    //shared_ptr<FuzzyLogic> flAngle = make_shared<FuzzyLogic>();
};