#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
//#include "Manager.h"
#include "../../lib/glm/vec3.hpp"
#include "../Facade/Render/RenderFacadeManager.h"

class BoxPowerUp;
class Data;

using namespace std;

class ManBoxPowerUp{  
   public:
    ManBoxPowerUp();
    ~ManBoxPowerUp();

    void CreateBoxPowerUp(glm::vec3 _position);
    void CreateBoxPowerUp();
    vector<shared_ptr<BoxPowerUp>> GetEntities() const {
        return BoxPowerUps;
    };

    void resetBox(BoxPowerUp* resetBox);

   private:
	vector<shared_ptr<BoxPowerUp>> BoxPowerUps;
    void SubscribeToEvents();
    void EjecutarMeHanCogido(Data d);
};