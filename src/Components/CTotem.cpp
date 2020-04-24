#include "CTotem.h"
#include "GameValues.h"

CTotem::CTotem() {
    auto time = GameValues::GetInstance()->GetTimeTotem();
    SEGUNDOS = time;
    m_compType = CompType::TotemComp;  
}

CTotem::~CTotem(){

}  