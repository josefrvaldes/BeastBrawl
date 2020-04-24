#pragma once

#include "ManNavMesh.h"

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <Aliases.h>
#include "../Systems/SystemOnline.h"
//#include "ManNavMesh.h"

using namespace std;
#define PI 3.141592

class Totem;
class Data;
class ManNavMesh;

class ManTotem : public Manager{
   public:
    //using ManNavMesh::CalculateNavMesh;
    ManTotem(ManNavMesh *);
    ~ManTotem();

    void CreateTotem();
    void CreateTotem(glm::vec3 _position);
    void Update();
    void SetSystemOnline(SystemOnline* systOn){ systemOnline = systOn; };
    //shared_ptr<Totem>& GetTotem() { return totem; };
    //vector<shared_ptr<Totem>> GetEntities() const { return totems; };

   private:
    //shared_ptr<Totem> totem;
    //vector<shared_ptr<Totem>> totems;
    void ActivateTotem(const glm::vec3 &position, int currentNavMesh);
    void AppertainCar(DataMap* d);
    void ResetTotem(DataMap* d);
    void SyncTotem(DataMap* d);
    void RecievedCatchTotem(DataMap* d);
    void RecievedLostTotem(DataMap* d);
    void SubscribeToEvents();

    ManNavMesh* manNavMesh;
    SystemOnline* systemOnline = nullptr; // en caso de que sea Single va a ser un nullptr
};