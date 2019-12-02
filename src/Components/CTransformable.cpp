#include "CTransformable.h"

CTransformable::CTransformable(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale)
{
    m_compType = CompType::TransformableComp;

    position = _pos;
    rotation = _rot;
    scale    = _scale;
}