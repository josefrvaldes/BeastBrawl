#pragma once

#include "Component.h"

#include <string>
#include <iostream>

class CShader : public Component{
public:
    CShader(std::string, std::string);
    ~CShader(){};


    std::string vertexShader;
    std::string fragmentShader;
};
