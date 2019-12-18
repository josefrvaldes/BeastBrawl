#include "CWheel.h"

#include "CId.h"


CWheel::CWheel(){
    m_compType = CompType::WheelComp;
}



CWheel::CWheel(float _offsetX,float _offsetZ) : offsetX(_offsetX), offsetZ(_offsetZ){
    m_compType = CompType::WheelComp;
    
    shared_ptr<CId> cIdTL  = make_shared<CId>();
    shared_ptr<CId> cIdTR   = make_shared<CId>();
    shared_ptr<CId> cIdBL   = make_shared<CId>();
    shared_ptr<CId> cIdBR   = make_shared<CId>();
    IdWheelTopLeft = cIdTL->id;
    IdWheelTopRight = cIdTR->id;
    IdWheelBottomLeft = cIdBL->id;
    IdWheelBottomRight = cIdBR->id;
}
