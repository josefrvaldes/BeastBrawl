#pragma once

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Entity;
class Camera;

class SystemLoD {
   public:
    SystemLoD() = default;
    ~SystemLoD() = default;

    void UpdateMeshes(vector<shared_ptr<Entity>> entities, Camera *cam);
    void UpdateAnimations(vector<shared_ptr<Entity>> entities, Camera *cam);

   private:
};