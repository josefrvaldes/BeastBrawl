#pragma once

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Component.h"

class Animation {
   public:
    //! Constructor de la clase animación
    //! @param path_ Ruta y nombre del archivo de malla, es decir, el obj. El formato será algo parecido a '/media/coche.obj'
    //! @param numKeyFrames_ El número de keyframes que hay en la animación
    Animation(std::string path_, uint8_t numKeyFrames_)
        : path{path_}, numKeyFrames{numKeyFrames_}, isInterpolated{false} {};

    //! Constructor de la clase animación
    //! @param path_ Ruta y nombre del archivo de malla, es decir, el obj. El formato será algo parecido a '/media/coche.obj'
    //! @param numKeyFrames_ El número de keyframes que hay en la animación
    //! @param distances_ vector de distancias en fps entre frame y frame. Su size tiene que coincidir con numKeyFrames.
    Animation(std::string path_, uint8_t numKeyFrames_, std::vector<uint8_t> distances_)
        : path{path_}, numKeyFrames{numKeyFrames_}, distances{distances_}, isInterpolated{true} {};

    std::string path;
    uint8_t numKeyFrames;
    std::vector<uint8_t> GetDistances() { return distances; };
    bool IsInterpolated() { return isInterpolated; };


   private:
    std::vector<uint8_t> distances;
    bool isInterpolated{false};
};

class CAnimation : public Component {
   public:
    CAnimation() = delete;
    CAnimation(Animation animClose);
    CAnimation(Animation animClose, Animation animMedium, Animation animFar);
    ~CAnimation(){};

    //float distanceNear{250};
    //float distanceMedium{400};
    std::vector<std::shared_ptr<Animation>> GetAnimations() { return animations; };
    // Animation GetActiveAnimation() { return activeAnimation; };

    float distanceNear{5000};
    float distanceMedium{10000};
    std::shared_ptr<Animation> activeAnimation;
    Animation *previousAnimation;

   private:

    std::vector<std::shared_ptr<Animation>> animations;
};
