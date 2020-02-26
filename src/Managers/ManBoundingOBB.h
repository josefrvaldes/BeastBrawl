#pragma once

#include "Manager.h"
#include "../../include/glm/vec3.hpp"

using namespace std;
using namespace glm;

struct CBoundingPlane;

class ManBoundingOBB : public Manager {
   public:
    ManBoundingOBB();
    ~ManBoundingOBB();
    void CreateBoundingOBB(const vector<glm::vec3> &, const vector<glm::vec3> &);   // Debemos pasarle todos los vertices que componente nuestro CUBO 3D
    void Integrate(float) override;
    void SubscribeToEvents() override;

   private:
   //vector<shared_ptr<CBoundingPlane>> planes;
   double EuclideanDis(const glm::vec3 &p1, const glm::vec3 &p2) const;
};
