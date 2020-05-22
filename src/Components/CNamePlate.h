#pragma once

#include "Component.h"

#include <string>
#include <iostream>

class CNamePlate : public Component{
public:
    CNamePlate(int _id,std::string _billboardPath);
    ~CNamePlate(){};


    int idCarAsociated;
    std::string billboardPath;
};
