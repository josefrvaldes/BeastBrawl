#pragma once

#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "Component.h"

class Animation {
   public:
    //! Constructor de la clase animación
    //! @param path_ Ruta y nombre del archivo de malla, es decir, el obj. El formato será algo parecido a '/media/coche.obj'
    //! @param numKeyFrames_ El número de keyframes que hay en la animación
    //! @param distances_ vector que representa la cantidad de frames que hay entre cada keyframe. El size debe coincidir con el numKeyFrames.
    Animation(std::string path_, uint8_t numKeyFrames_, std::vector<uint8_t> distances_)
        : path{path_}, numKeyFrames{numKeyFrames_}, distances{distances_} {};
    std::string path;
    uint8_t numKeyFrames;
    std::vector<uint8_t> distances;
};

class CAnimation : public Component {
   public:
    CAnimation() = delete;
    CAnimation(Animation animClose);
    CAnimation(Animation animClose, Animation animMedium, Animation animFar);

    // TODO: implementar animaciones con LoD
    // CAnimation(std::string pathCerca, std::string pathMedio, std::string pathLejos, uint8_t numKeyFrames);
    ~CAnimation(){};

    std::vector<Animation> animations;
    Animation activeAnimation;

    //float distanceNear{250};
    //float distanceMedium{400};

    float distanceNear{5000};
    float distanceMedium{10000};
};
