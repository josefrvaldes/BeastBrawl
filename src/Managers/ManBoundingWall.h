#pragma once

#include "Manager.h"

#include <glm/vec3.hpp>

using namespace std;

class ManBoundingWall : public Manager {
   public:
    ManBoundingWall();
    ~ManBoundingWall();
    void CreateBoundingWall(glm::vec3 vertexA, glm::vec3 vertexB, glm::vec3 vertexC, glm::vec3 vertexD);
    void Integrate(float) override;
    void SubscribeToEvents() override;

   private:
};
