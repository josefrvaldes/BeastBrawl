#pragma once


#include "Component.h"

using namespace std;

class CParentNode : public Component{
public:
    CParentNode(uint16_t id);
    ~CParentNode();

    uint16_t idParentNode = 0;
};