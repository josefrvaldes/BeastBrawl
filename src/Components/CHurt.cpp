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
 
 
 #include "CHurt.h"

#include "CCar.h"
#include "CTransformable.h"



CHurt::CHurt() {
    m_compType = CompType::HurtComp;
}

void CHurt::Hurt(CCar *cCar, CTransformable *cTransformable) {
    // si la rotación de ruedas es negativa, significa que estábamos girando a izquierda
    if (cCar->wheelRotation < 0) {
        SetLeft(true);
    } else {
        SetLeft(false);
    }
    hurt = true;
    originalCarRotation = cTransformable->rotation.y;
}

void CHurt::SetLeft(bool left_) {
    left = left_;
    if(left_) {
        leftMultiplier = -1;
    } else {
        leftMultiplier = 1;
    }
}