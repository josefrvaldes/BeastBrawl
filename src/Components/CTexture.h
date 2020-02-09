#pragma once

#include "Component.h"

#include <string>
#include <iostream>

class CTexture : public Component{
public:
    CTexture(std::string);
    ~CTexture(){};

    std::string texture;
};