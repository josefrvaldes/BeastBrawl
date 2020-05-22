/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
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
    Animation(std::string path_, uint8_t numKeyFrames_, bool loop_)
        : path{path_}, numKeyFrames{numKeyFrames_}, loop{loop_}, isInterpolated{false} {};

    //! Constructor de la clase animación
    //! @param path_ Ruta y nombre del archivo de malla, es decir, el obj. El formato será algo parecido a '/media/coche.obj'
    //! @param numKeyFrames_ El número de keyframes que hay en la animación
    //! @param distances_ vector de distancias en fps entre frame y frame. Su size tiene que coincidir con numKeyFrames.
    Animation(std::string path_, uint8_t numKeyFrames_, bool loop_, std::vector<uint8_t> distances_)
        : path{path_}, numKeyFrames{numKeyFrames_}, loop{loop_}, distances{distances_}, isInterpolated{true} {};

    std::string path;
    uint8_t numKeyFrames;
    bool loop;
    std::vector<uint8_t> GetDistances() { return distances; };
    bool IsInterpolated() { return isInterpolated; };

   private:
    std::vector<uint8_t> distances;
    bool isInterpolated{false};
};

class CAnimation : public Component {
   public:
    CAnimation();
    // CAnimation(Animation animClose);
    // CAnimation(Animation animClose, Animation animMedium, Animation animFar);
    ~CAnimation(){};

    void AddAnimationWin(Animation animWin_) {
        animWin = std::make_shared<Animation>(animWin_);
    };

    void AddAnimationIdle(Animation animIdle_) {
        animIdle = std::make_shared<Animation>(animIdle_);
    };

    void AddAnimationLeft(Animation animLeft_) {
        animLeft = std::make_shared<Animation>(animLeft_);
    };

    void AddAnimationRight(Animation animRight_) {
        animRight = std::make_shared<Animation>(animRight_);
    };

    void AddAnimationDamage(Animation animDamage_) {
        animDamage = std::make_shared<Animation>(animDamage_);
    };

    void ActivateAnimationLeft() {
        if (activeAnimation.get() != animLeft.get()) {
            activeAnimation = animLeft;
            animationChanged = true;
        }
    }

    void ActivateAnimationRight() {
        if (activeAnimation.get() != animRight.get()) {
            activeAnimation = animRight;
            animationChanged = true;
        }
    }

    void ActivateAnimationWin() {
        if (activeAnimation.get() != animWin.get()) {
            activeAnimation = animWin;
            animationChanged = true;
        }
    }

    void ActivateAnimationDamage() {
        if (activeAnimation.get() != animDamage.get()) {
            activeAnimation = animDamage;
            animationChanged = true;
        }
    }

    void ActivateAnimationIdle() {
        if (activeAnimation.get() != animIdle.get()) {
            activeAnimation = animIdle;
            animationChanged = true;
        }
    }

    std::shared_ptr<Animation> GetAnimWin() {return animWin;};
    std::shared_ptr<Animation> GetAnimIdle() {return animIdle;};
    std::shared_ptr<Animation> GetAnimRight() {return animRight;};
    std::shared_ptr<Animation> GetAnimLeft() {return animLeft;};
    std::shared_ptr<Animation> GetAnimDamage() {return animDamage;};

    //float distanceNear{250};
    //float distanceMedium{400};
    std::vector<std::shared_ptr<Animation>> GetAnimations() { return animations; };
    // Animation GetActiveAnimation() { return activeAnimation; };

    bool animationChanged{true};
    float distanceNear{5000};
    float distanceMedium{10000};
    std::shared_ptr<Animation> activeAnimation{nullptr};
    // Animation *previousAnimation{nullptr};

   private:
    std::vector<std::shared_ptr<Animation>> animations;
    std::shared_ptr<Animation> animWin;
    std::shared_ptr<Animation> animIdle;
    std::shared_ptr<Animation> animRight;
    std::shared_ptr<Animation> animLeft;
    std::shared_ptr<Animation> animDamage;
};
