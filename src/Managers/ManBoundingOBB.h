#pragma once

#include "Manager.h"
#include "../../include/glm/vec3.hpp"

using namespace std;
using namespace glm;

class ManBoundingOBB : public Manager {
   public:
    ManBoundingOBB();
    ~ManBoundingOBB();
    void CreateBoundingOBB(const vector<vec3> &, const vec3 &);   // Debemos pasarle todos los vertices que componente nuestro CUBO 3D
    void Integrate(float) override;
    void SubscribeToEvents() override;

   private:
};
