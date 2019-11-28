#include "CId.h"

uint16_t CId::numIds = 0;

CId::CId(){
    m_compType = CompType::IdComp;
    m_id = numIds;

    cout << "Creado componente con ID: " << numIds << endl;
    numIds++;

}


CId::~CId(){

    
}