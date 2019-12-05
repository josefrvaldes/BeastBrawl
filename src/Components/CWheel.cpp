#include "CWheel.h"

#include "CId.h"


CWheel::CWheel(){
    m_compType = CompType::WheelComp;
}



CWheel::CWheel(float _offsetX,float _offsetZ) : offsetX(_offsetX), offsetZ(_offsetZ){
    m_compType = CompType::WheelComp;
    
    CId* cIdTL  = new CId();
    CId* cIdTR   = new CId();
    CId* cIdBL   = new CId();
    CId* cIdBR   = new CId();
    IdWheelTopLeft = cIdTL->id;
    IdWheelTopRight = cIdTR->id;
    IdWheelBottomLeft = cIdBL->id;
    IdWheelBottomRight = cIdBR->id;
}
