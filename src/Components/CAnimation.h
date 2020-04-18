#pragma once

#include "Component.h"

#include <iostream>
#include <string>
#include <vector>

class CAnimation : public Component {
   public:
    CAnimation() = delete;
    CAnimation(std::string fileName, uint8_t numKeyFrames_);
    
    // TODO: implementar animaciones con LoD
    // CAnimation(std::string pathCerca, std::string pathMedio, std::string pathLejos, uint8_t numKeyFrames);
    ~CAnimation(){};

    std::string mesh;
    uint8_t numKeyFrames;

    //float distanceNear{250};
    //float distanceMedium{400};

    float distanceNear{5000};
    float distanceMedium{10000};
};
