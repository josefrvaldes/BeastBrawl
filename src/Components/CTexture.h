#pragma once

#include "Component.h"
#include <string>
#include <iostream>

class CTexture : public Component{
public:
    CTexture(std::string);
    ~CTexture();

    std::string GetTexture() { return m_texture;};

private:
    std::string m_texture;
};