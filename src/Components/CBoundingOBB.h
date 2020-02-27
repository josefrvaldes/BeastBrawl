#pragma once

#include "../../include/glm/geometric.hpp"
#include "../../include/glm/vec3.hpp"
#include "../CLPhysics/IntersectData.h"

#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include <vector>
#include "Component.h"
#include <memory>



using namespace glm;
using namespace std;

class CBoundingSphere;
class IntersectData;
struct CCar;
struct CTransformable;
struct CBoundingPlane;

class CBoundingOBB : public Component {
   public:
    CBoundingOBB(const vector<glm::vec3> &, const vector<glm::vec3> &);

    // void Normalize();
    IntersectData IntersectSphere(const CBoundingSphere &sphere, const CTransformable &trCar, const CCar &ccarCar);

    IntersectData IntersectPlane(const CBoundingPlane &other) const;

    IntersectData IntersectRay(const glm::vec3 &posRayOrigin, const glm::vec3 &rayNormalNormalized) const;

    vector<shared_ptr<CBoundingPlane>> planes;
    vector<glm::vec3> centersMass;
    // Recordatorio, un plano tiene 4 vec3 (sus 4 vertices), su normal, su ecuacion general, y una distancia.

    private:
    double EuclideanDis(const glm::vec3 &p1, const glm::vec3 &p2) const;
};