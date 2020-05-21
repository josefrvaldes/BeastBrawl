#include "CCamera.h"

CCamera::CCamera(float _tarx, float _tary, float _tarz, float _rotExtraY)
: tarX(_tarx), tarY(_tary), tarZ(_tarz), rotExtraY(_rotExtraY)
{
    m_compType = CompType::CameraComp;
    this->ApplyPhysics(0.0,0.0,0.0, 0.0);

    float perfectUpDistance {25.1}; // 22
    float upTargetDistance {18.3};  // 16
}  
 

// vamos a aplicar fisicas a la camara, lo que implica que tendra su propio movimiento, siempre dependiente de alguna entidad
// pero con sus propios parametros de velocidad, aceleracion y frenado.
void CCamera::ApplyPhysics(float maxSpeed_, float acc_, float revMaxSpeed_, float slowDown_){

    maxSpeed        = maxSpeed_;        // velocidad maxima que la entidad
    acceleration    = acc_*0.9;         // aceleracion un poco mas lenta que la de la entidad
    reverseMaxSpeed = revMaxSpeed_;     // marcha atras igual que la entidad
    slowDown        = slowDown_;
}