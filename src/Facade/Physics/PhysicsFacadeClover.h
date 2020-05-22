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

#include "PhysicsFacade.h"
#include "../Render/RenderFacadeManager.h"

#include "../../../CLEngine/src/CLEngine.h"


#include <iostream>
#include <memory>

using namespace CLE;
using namespace std;
class PhysicsFacadeClover : public PhysicsFacade {
   public:
    PhysicsFacadeClover();
    ~PhysicsFacadeClover();
    void UpdateCar(Entity*, Entity*) override;
    void UpdateTransformable(Entity*) override;
    void UpdatePowerUps(Entity*) override;
    void UpdateShields(const vector<shared_ptr<Entity>>&, const vector<shared_ptr<Entity>>&) override;
    void SetWindowContext(CLEngine* d) { device = d;};

   private:
    void UpdateCam(Entity*);
    CLEngine* device;
    CLNode* smgr;
};