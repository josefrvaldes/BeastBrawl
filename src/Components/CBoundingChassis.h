#pragma once

#include "../../include/glm/geometric.hpp"
#include "../../include/glm/vec3.hpp"
#include "../../include/glm/vec4.hpp"
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
struct CBoundingCilindre;

class CBoundingChassis : public Component {
   public:
    CBoundingChassis(const vec3 &spCenterBehind, const float &spRadiusBehind, const vec3 &spCenterFront, const float &spRadiusFront, 
                        const vector<vec3> &plRight, const vector<vec3> &plLeft);

    unique_ptr<CBoundingSphere> sphereBehind;
    unique_ptr<CBoundingSphere> sphereFront;
    unique_ptr<CBoundingCilindre> cilindre;
};