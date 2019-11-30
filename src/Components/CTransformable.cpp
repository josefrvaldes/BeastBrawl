#include "CTransformable.h"

CTransformable::CTransformable(float _posx, float _posy, float _posz, float _rotx, float _roty, float _rotz, float _scalex, float _scaley, float _scalez)
: posX(_posx), posY(_posy), posZ(_posz), rotX(_rotx), rotY(_roty), rotZ(_rotz), scaleX(_scalex), scaleY(_scaley), scaleZ(_scalez)
{
    m_compType = CompType::TransformableComp;
}