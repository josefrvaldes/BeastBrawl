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
 
 
 #include "SystemAnimationEnd.h"

#include "../Entities/Camera.h"
#include "../Entities/Car.h"
#include "../Entities/Totem.h"
#include "../Systems/Utils.h"

SystemAnimationEnd::SystemAnimationEnd(Camera *cam_) : cam{cam_}, winner{nullptr}, timeStartAnim{Utils::getMillisSinceEpoch()} {
}

void SystemAnimationEnd::Animate() {
    MoveCam(0, 0);
}

void SystemAnimationEnd::MoveCam(int64_t interval, int64_t duration) {
    auto cTransforCar = static_cast<CTransformable *>(winner->GetComponent(CompType::TransformableComp).get());
    auto cTransforCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    cTransforCam->position = cTransforCar->position;
    cTransforCam->position.y += 20;
    cTransforCam->position.x += sin(glm::radians(rotation)) * 30;
    cTransforCam->position.z += cos(glm::radians(rotation)) * 30;
    rotation += ROTATION_SPEED;
}

void SystemAnimationEnd::ResetTimer() {
    timeStartAnim = Utils::getMillisSinceEpoch();
}

void SystemAnimationEnd::SetWinner(Car *winner_) {
    winner = winner_;
    auto cTransformable = static_cast<CTransformable *>(winner->GetComponent(CompType::TransformableComp).get());
    cam->setTarget(cTransformable->position);
}