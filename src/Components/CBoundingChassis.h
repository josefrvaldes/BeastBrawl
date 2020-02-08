#pragma once

#include "../../include/glm/geometric.hpp"
#include "../../include/glm/vec3.hpp"

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
struct CBoundingCilindre;

class CBoundingChassis : public Component {
   public:
    CBoundingChassis(const vec3 &spCenterBehind, const float &spRadiusBehind, const vec3 &spCenterFront, const float &spRadiusFront);

    shared_ptr<CBoundingSphere> sphereBehind;
    shared_ptr<CBoundingSphere> sphereFront;
    shared_ptr<CBoundingCilindre> cilindre;
};