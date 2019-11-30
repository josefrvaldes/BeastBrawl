#include "CCamera.h"

CCamera::CCamera(float _tarx, float _tary, float _tarz, float _rotExtraY)
: tarX(_tarx), tarY(_tary), tarZ(_tarz), rotExtraY(_rotExtraY)
{
    m_compType = CompType::CameraComp;
}