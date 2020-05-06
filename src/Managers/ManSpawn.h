#pragma once

#include "Manager.h"

#include <glm/vec3.hpp>

using namespace std;

class ManSpawn{
   public:
    ManSpawn();
    ~ManSpawn();

    auto GetPosSpawn() { return posSpawn; };

   private:
    vector<glm::vec3> posSpawn;
};
