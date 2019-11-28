#include "CCamera.h"

CCamera::CCamera(float tarx, float tary, float tarz, float rotExtraY)
: m_tarx(tarx), m_tary(tary), m_tarz(tarz), m_rotExtraY(rotExtraY)
{
    m_compType = CompType::CameraComp;
}