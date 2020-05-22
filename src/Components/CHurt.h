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

#include "Component.h"

class CCar;
class CTransformable;

class CHurt : public Component {
   public:
    CHurt();
    ~CHurt(){};

    float currentRotation{0};  // indica la rotación actual que se le sumará al coche
    float originalCarRotation{0};
    const float speedRotationHigh{18};
    const float speedRotationLow{5};
    bool hurt{false};
    const float FINAL_ROTATION{720};

    void Hurt(CCar *, CTransformable *cTrans);
    void SetLeft(bool left_);
    bool GetLeft() { return left; };
    int8_t GetLeftMultiplier() { return leftMultiplier; };
    
   private:
    int8_t leftMultiplier = 1;
    bool left{false};

};