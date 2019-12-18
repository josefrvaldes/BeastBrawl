#include "CPath.h"

CPath::CPath(){
    m_compType = CompType::PathComp;
}

CPath::CPath(stack<int> path){
    m_compType = CompType::PathComp;
    stackPath = path;
}