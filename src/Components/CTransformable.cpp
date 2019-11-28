#include "CTransformable.h"

CTransformable::CTransformable(float posx, float posy, float posz, float rotx, float roty, float rotz, float scalex, float scaley, float scalez)
: m_posx(posx), m_posy(posy), m_posz(posz), m_rotx(rotx), m_roty(roty), m_rotz(rotz), m_scalex(scalex), m_scaley(scaley), m_scalez(scalez)
{
    m_compType = CompType::TransformableComp;
}