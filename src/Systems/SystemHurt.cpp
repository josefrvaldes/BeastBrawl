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
 
 
 #include "SystemHurt.h"

#include "../Entities/Entity.h"
#include "../Components/CHurt.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "../../include/glm/glm.hpp"
#include "Utils.h"

void SystemHurt::Update(vector<shared_ptr<Entity>> entities) {
    for (auto e : entities) {
        auto cHurt = static_cast<CHurt*>(e->GetComponent(CompType::HurtComp).get());
        if(cHurt->hurt) {
            auto cTransformable = static_cast<CTransformable*>(e->GetComponent(CompType::TransformableComp).get());
            // si hemos acabado las dos vueltas...
            if(cHurt->currentRotation >= cHurt->FINAL_ROTATION || cHurt->currentRotation <= -cHurt->FINAL_ROTATION) {
                // ponemos hurt a false y reiniciamos estado
                cHurt->hurt = false;
                cHurt->currentRotation = 0;
                cTransformable->rotation.y = cHurt->originalCarRotation;
                

                // si estamos dando vueltas..
            } else {
                // le quitamos la velocidad al coche
                auto cCar = static_cast<CCar*>(e->GetComponent(CompType::CarComp).get());
                cCar->speed = 0;

                // interpolamos la velocidad de giro para que empiece girando rápido y acabe girando lento
                float percentTick = std::min(1.0f, std::fabs(cHurt->currentRotation) / cHurt->FINAL_ROTATION);
                float currentSpeed = mix(cHurt->speedRotationHigh, cHurt->speedRotationLow, percentTick) * cHurt->GetLeftMultiplier();


                // y finalmente aplicamos la rotación.
                cHurt->currentRotation += currentSpeed;
                
                
                // cout << "currentRotation[" << cHurt->currentRotation << "]" << endl;
                cTransformable->rotation.y = cHurt->originalCarRotation + cHurt->currentRotation;
            }
        }
    }
}