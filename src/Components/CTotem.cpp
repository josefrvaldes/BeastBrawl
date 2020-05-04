#include "CTotem.h"
#include "GameValues.h"

CTotem::CTotem() : SEGUNDOS{GameValues::GetInstance()->GetTimeTotem()}, DURATION_TIME{/*SEGUNDOS*/ 5 * 1000} {
    m_compType = CompType::TotemComp;  
}

CTotem::~CTotem(){

}  