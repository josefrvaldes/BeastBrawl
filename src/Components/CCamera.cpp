#include "CCamera.h"

CCamera::CCamera(float tarx, float tary, float tarz, float rotExtraY){
    m_compType = CompType::CameraValues;
    m_tarx = tarx;
    m_tary = tary;
    m_tarz = tarz;

    m_rotExtraY = rotExtraY;

}