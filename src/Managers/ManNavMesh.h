#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Manager.h"
#include "ManCar.h"
#include "../../lib/glm/vec3.hpp"
#include "../Facade/Render/RenderFacadeManager.h"

class NavMesh;

using namespace std;

class ManNavMesh : public Manager{
   public:
    ManNavMesh();
    ~ManNavMesh(){};

    void CreateNavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, vector<int> waypoints);
    void CreateNavMesh();

    void UpdateNavMeshTotem(Manager* manTotem);
    //void UpdateNavMeshCarsAI(ManCar* manCars);

    // vector<shared_ptr<NavMesh>> GetEntities() const {
    //     return navMeshes;
    // };

    // vector<shared_ptr<NavMesh>> GetEntities()  {
    //     return navMeshes;
    // };


   private:
    //vector<shared_ptr<NavMesh>> navMeshes;

};