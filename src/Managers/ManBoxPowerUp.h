#pragma once

#include "Manager.h"

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <Facade/Render/RenderFacadeManager.h>

#include <Components/CBoxPowerUp.h>
#include <Components/CId.h>

class BoxPowerUp;
class Data;

using namespace std;

class ManBoxPowerUp : public Manager{
   public:
    ManBoxPowerUp();
    ~ManBoxPowerUp();

    void CreateBoxPowerUp(glm::vec3 _position);
    void CreateBoxPowerUp();
    //vector<shared_ptr<BoxPowerUp>> GetEntities() const {
    //    return BoxPowerUps;
    //};

    void resetBox(Entity* resetBox);

   private:
    //vector<shared_ptr<BoxPowerUp>> BoxPowerUps;
    void SubscribeToEvents() override;
    void EjecutarMeHanCogido(DataMap* d);
};