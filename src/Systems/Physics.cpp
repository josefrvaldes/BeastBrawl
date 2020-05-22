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
 * @author Antonio Jose Martinez Garcia
 * 
 */
 
 
 #include "Physics.h"
#include <Components/CCar.h>
#include <Components/CNitro.h>
#include <Components/CSpeed.h>
#include <Components/CBufferOnline.h>
#include <Entities/Car.h>
#include "../Entities/CarHuman.h"
#include "../Components/COnline.h"
#include "../Components/CExternalForce.h"
#include "../Systems/Utils.h"
#include "../../include/glm/glm.hpp"

#include <cmath>

#include <iostream>
#include <math.h>

#include "../Constants.h"

Physics::Physics(float _deltaTime) : deltaTime(_deltaTime) {
}

void Physics::update(Car *car) {
    // actualizar posiciones
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    //auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());
    //auto cTransformableCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    auto cExternalForce = static_cast<CExternalForce *>(car->GetComponent(CompType::CompExternalForce).get());

    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, cSpeed, cExternalForce, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, cExternalForce, deltaTime);
    //CalculatePositionCamera(cCar, cTransformable, cTransformableCam, cCamera);
}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void Physics::CalculatePosition(CCar *cCar, CTransformable *cTransformable, CSpeed *cSpeed, CExternalForce *cExternalForce, float deltaTime) {
    // debemos de tener encuenta la fuerza externa, asi como la direccion final que tomaremos (el angulo final)
    if(cExternalForce->force > 0){
        // Este paso es una tonteria porque ya lo devolvemos normalizado
        cExternalForce->dirExternalForce = normalize(cExternalForce->dirExternalForce);
    }  

    float angleRotation = cTransformable->rotation.y - cCar->skidRotation;
    angleRotation = Utils::GetAdjustedDegrees(angleRotation);

    cSpeed->speed.x = cos(glm::radians(angleRotation));  // * cCar->speed;
    cSpeed->speed.z = sin(glm::radians(angleRotation));  // * cCar->speed;
    glm::vec2 carForce(-cos(glm::radians(angleRotation))*cCar->speed , sin(glm::radians(angleRotation))*cCar->speed);  // Fijarse en el menos de la X
    glm::vec2 finalForce = ApplyExternalForce(cCar, cExternalForce, carForce);

    // Movimiento del coche
    cTransformable->position.x += finalForce.x * deltaTime;
    cTransformable->position.z += finalForce.y * deltaTime;

    // Rotacion del coche
    if(cCar->skidRotation != 0){
        cTransformable->rotation.y += cCar->wheelRotation * 0.05 + 0.03 * cCar->skidRotation;
    }else{
        cTransformable->rotation.y += cCar->wheelRotation * 0.20;
    }
    cTransformable->rotation.y = Utils::GetAdjustedDegrees(cTransformable->rotation.y);
}


vec3 Physics::CalculateVecDirCar(CTransformable *cTransformable) const{
   float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
   float nextPosX    = cTransformable->position.x - cos(angleRotation) * 1;
   float nexPosZ     = cTransformable->position.z + sin(angleRotation) * 1;

   return vec3(nextPosX-cTransformable->position.x, 0, nexPosZ-cTransformable->position.z);
}


//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void Physics::CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, CExternalForce *cExternalForce, float deltaTime) {
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float delta = deltaTime;

    glm::vec2 carForce(-cos(angleRotation)*cCar->speed , sin(angleRotation)*cCar->speed);  // Fijarse en el menos de la X
    glm::vec2 finalForce = ApplyExternalForce(cCar, cExternalForce, carForce);

    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->position.x += finalForce.x * delta;
    cTransformable->position.z += finalForce.y * delta;

    //Si tiene rotacion, rotamos el coche
    if (cCar->wheelRotation != 0) {
        cTransformable->rotation.y -= cCar->wheelRotation * 0.20;
        cTransformable->rotation.y = Utils::GetAdjustedDegrees(cTransformable->rotation.y);
    }
}


// aplicamos al movimiento del coche el desplazamiento en caso de que alguien lo empuje
glm::vec2 Physics::ApplyExternalForce(CCar *cCar, CExternalForce *externalForce, const glm::vec2& carForce) const{
    glm::vec2 finalForce(carForce);
    if(externalForce->force > 0){
        glm::vec2 collisionForce(externalForce->dirExternalForce.x*externalForce->force, externalForce->dirExternalForce.z*externalForce->force);
        finalForce.x = carForce.x + collisionForce.x;
        finalForce.y = carForce.y + collisionForce.y;
        externalForce->force -= externalForce->friction;
    }else{
        externalForce->force = 0.0;
    }
    return finalForce;
}



//Entra cuando se presiona la I
void Physics::Accelerate(Car *car) {
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    //Aumentamos la velocidad
    if (cNitro->activePowerUp == false) {
        cCar->speed += cCar->acceleration;
        if (cCar->speed > cCar->maxSpeed) {
            cCar->speed -= cCar->acceleration * 4.0;
            if (cCar->speed < cCar->maxSpeed)
                cCar->speed = cCar->maxSpeed;
        }
        // cout << "La current speed es " << cCar->speed << endl;
    } else {
        cCar->speed += cNitro->nitroAcceleration;
        if (cCar->speed > cNitro->nitroMaxSpeed) {
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }
}

//Entra cuando se presiona la O
void Physics::Decelerate(Car *car) {
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    //Reducimos la velocidad
    if (cNitro->activePowerUp == false) {
        cCar->speed -= cCar->slowDown;
        if (cCar->speed < cCar->reverseMaxSpeed) {  // no se supere la velocidad marcha atras
            cCar->speed = cCar->reverseMaxSpeed;
        }
    } else {
        cCar->speed += cNitro->nitroAcceleration;
        if (cCar->speed > cNitro->nitroMaxSpeed) {
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }
}

//Entra cuando se presiona la A
void Physics::TurnLeft(Car *car) {
    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->speed >= cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation > -cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= cCar->incrementWheelRotation;
        }
    } else if (cCar->speed <= -cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation > -cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= cCar->incrementWheelRotation;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar
        if (cCar->wheelRotation >= cCar->decrementWheelRotation) {
            cCar->wheelRotation -= cCar->decrementWheelRotation;
        } else if (cCar->wheelRotation <= -cCar->decrementWheelRotation) {
            cCar->wheelRotation += cCar->decrementWheelRotation;
        } else {
            cCar->wheelRotation = 0;
        }
    }

    if(cCar->skidState == SkidState::SKID_START && cCar->speed>cCar->maxSpeed*0.6 && duration_cast<milliseconds>(system_clock::now() - cCar->skidStart).count() < cCar->skidActivationTime){
        cCar->skidState = SkidState::SKID_TO_LEFT;
        cCar->skidDeg = cCar->skidDegL;
    }
}

//Entra cuando se presiona la D
void Physics::TurnRight(Car *car) {
    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->speed >= cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation < cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += cCar->incrementWheelRotation;
        }
    } else if (cCar->speed <= -cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation < cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += cCar->incrementWheelRotation;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar
        if (cCar->wheelRotation >= cCar->decrementWheelRotation) {
            cCar->wheelRotation -= cCar->decrementWheelRotation;
        } else if (cCar->wheelRotation <= -cCar->decrementWheelRotation) {
            cCar->wheelRotation += cCar->decrementWheelRotation;
        } else {
            cCar->wheelRotation = 0;
        }
    }

    if(cCar->skidState == SkidState::SKID_START && cCar->speed>cCar->maxSpeed*0.6 && duration_cast<milliseconds>(system_clock::now() - cCar->skidStart).count() < cCar->skidActivationTime){
        cCar->skidState = SkidState::SKID_TO_RIGHT;
        cCar->skidDeg = cCar->skidDegR;
    }
}

//Aqui entra cuando no se esta presionando ni I ni O
void Physics::NotAcceleratingOrDecelerating(Car *car) {
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());

    if (cNitro->activePowerUp == false) {
        if (cCar->speed > 0) {
            cCar->speed -= cCar->friction;
            if (cCar->speed < 0)
                cCar->speed = 0;
        } else if (cCar->speed < 0) {
            cCar->speed += cCar->friction;
            if (cCar->speed > 0)
                cCar->speed = 0;
        }
    } else {
        cCar->speed += cNitro->nitroAcceleration;
        if (cCar->speed > cNitro->nitroMaxSpeed) {
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }
}


//Aqui entra cuando no se esta presionando ni A ni D
void Physics::NotTurning(Car *car) {
    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->wheelRotation >= cCar->decrementWheelRotation) {
        cCar->wheelRotation -= cCar->decrementWheelRotation;
    } else if (cCar->wheelRotation <= -cCar->decrementWheelRotation) {
        cCar->wheelRotation += cCar->decrementWheelRotation;
    } else {
        cCar->wheelRotation = 0;
    }
}


// Metodo para cuando pulsamos el derrape
void Physics::Skid(Car *car){
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());

    if(cCar->skidState == SkidState::DISABLED ){ // Reiniciamos el reloj y comenzamos, en caso de pulsar una de las dos direcciones dentro de un tiempo comenzara el giro del derrape
        cCar->skidStart = system_clock::now();
        cCar->skidState = SkidState::SKID_START;
        EventManager::GetInstance().AddEventMulti(Event{EventType::DRIFTS});
    }

    // Si se mantiene pulsado va a incrementar hasta que se alcance la posicion
    if(cCar->skidState == SkidState::SKID_TO_LEFT){
        auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        cCar->skidRotation -= cCar->skidAcc;
        cTrans->rotation.y -= cCar->skidAcc;
        if(cCar->skidRotation < cCar->skidDeg){
            cCar->skidRotation += cCar->skidAcc;
            cTrans->rotation.y += cCar->skidAcc;
            cCar->skidState = SkidState::SKID_LEFT;
        }
        if(cTrans->rotation.y < 0) cTrans->rotation.y += 360.0;
    }else if(cCar->skidState == SkidState::SKID_TO_RIGHT){
        auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        cCar->skidRotation += cCar->skidAcc;
        cTrans->rotation.y += cCar->skidAcc;
        if(cCar->skidRotation > cCar->skidDeg){
            cCar->skidRotation -= cCar->skidAcc;
            cTrans->rotation.y -= cCar->skidAcc;
            cCar->skidState = SkidState::SKID_RIGHT;
        }
        if(cTrans->rotation.y >= 360) cTrans->rotation.y -= 360.0;  
    }
    if(cCar->speed<cCar->maxSpeed*0.5){
        if(cCar->skidState == SkidState::SKID_LEFT || cCar->skidState == SkidState::SKID_TO_LEFT)
            cCar->skidState = SkidState::SKID_RECOVER_LEFT;
        else if(cCar->skidState == SkidState::SKID_RIGHT || cCar->skidState == SkidState::SKID_TO_RIGHT)
            cCar->skidState = SkidState::SKID_RECOVER_RIGHT;
    }

    RecoverSkid(*cCar, *cTrans);
}


void Physics::NotSkidding(Car *car){
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());

    // En caso de dejar de pulsar el derrape va a tener un periodo de recuperacion del derrape
    if( cCar->skidState == SkidState::SKID_LEFT || cCar->skidState == SkidState::SKID_TO_LEFT)
        cCar->skidState = SkidState::SKID_RECOVER_LEFT;
    else if( cCar->skidState == SkidState::SKID_RIGHT || cCar->skidState == SkidState::SKID_TO_RIGHT)
        cCar->skidState = SkidState::SKID_RECOVER_RIGHT;

    RecoverSkid(*cCar, *cTrans);
    
    if(cCar->skidState == SkidState::SKID_START && duration_cast<milliseconds>(system_clock::now()-cCar->skidStart).count() > cCar->skidAnimationTime){
        cCar->skidState = SkidState::DISABLED;
        cCar->skidRotation = 0;
    }
}


void Physics::RecoverSkid(CCar &cCar, CTransformable &cTrans) const{
    if( cCar.skidState == SkidState::SKID_RECOVER_LEFT){
        cCar.skidRotation += cCar.skidRecoverAcc; 
        cTrans.rotation.y += cCar.skidRecoverAcc;
        if(cCar.skidRotation >= 0){
            cTrans.rotation.y -= cCar.skidRotation;
            cCar.skidRotation = 0;
            if(duration_cast<milliseconds>(system_clock::now()-cCar.skidStart).count() > cCar.skidActivationTime)
                cCar.skidState = SkidState::SKID_START;
        }
        cTrans.rotation.y = Utils::GetAdjustedDegrees(cTrans.rotation.y);
    }else if(cCar.skidState == SkidState::SKID_RECOVER_RIGHT){
        cCar.skidRotation -= cCar.skidRecoverAcc;
        cTrans.rotation.y -= cCar.skidRecoverAcc;
        if(cCar.skidRotation <= 0){
            cTrans.rotation.y -= cCar.skidRotation;
            cCar.skidRotation = 0;
            if(duration_cast<milliseconds>(system_clock::now()-cCar.skidStart).count() > cCar.skidActivationTime)
                cCar.skidState = SkidState::SKID_START;
        }
        cTrans.rotation.y = Utils::GetAdjustedDegrees(cTrans.rotation.y);
    }
}


void Physics::NewInputsReceivedOnline(Car *car, float speed, float wheelRotation, float skidDeg, float skidRotation, CBufferOnline *buffer) {
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    auto cOnline = static_cast<COnline *>(car->GetComponent(CompType::OnlineComp).get());
    auto cExternalForce = static_cast<CExternalForce *>(car->GetComponent(CompType::CompExternalForce).get());
    cCar->speed = speed;
    cCar->wheelRotation = wheelRotation;
    cCar->skidDeg = skidDeg;
    cCar->skidRotation = skidRotation;

    if(buffer->elems.size() > 1) {
        BuffElement elemRecienRecibido = buffer->elems.front();
        buffer->elems.pop_front();
        BuffElement elemSiguiente = buffer->elems.front();
        cTransformable->position = elemSiguiente.pos;
        cTransformable->rotation = elemSiguiente.rot;
        int32_t intervalo = elemRecienRecibido.time - elemRecienRecibido.timeSent;

        float veces = intervalo / Constants::DELTA_TIME_MILLIS;
        int16_t auxVeces = round(veces);

        // TODO: posible corrección de desfase. Parece que el online va siempre 1 frame por delante, así que para ajustar, haremos aquí
        // una corrección menos:
        for(int16_t i = 0; i < auxVeces - 1; i++) {
            MoveCarHumanByInput(car, cCar, cOnline, cTransformable, cSpeed, cNitro, cExternalForce);
        }
    }
}


void Physics::NewSyncReceivedOnline(Car *car, int64_t timeSent, int64_t gameTime) {
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    auto cOnline = static_cast<COnline *>(car->GetComponent(CompType::OnlineComp).get());
    auto cExternalForce = static_cast<CExternalForce *>(car->GetComponent(CompType::CompExternalForce).get());
    auto cBufferOnline = static_cast<CBufferOnline *>(car->GetComponent(CompType::BufferOnline).get());

    auto elems = cBufferOnline->elems;
    std::list<BuffElement>::iterator it;
    uint16_t veces = round(gameTime - timeSent) / (Constants::DELTA_TIME_MILLIS) + 1;   // este 1 es porque nosotros enviamos el sync en el input del frame actual, pero el 
                                                                                                        // input es lo primero que se hace del frame, osea que realmente estamos enviando la pos que se calculó el frame anterior
    for(uint16_t i = 0; i < veces; i++) {
        MoveCarHumanByInput(car, cCar, cOnline, cTransformable, cSpeed, cNitro, cExternalForce);
    }
}

void Physics::MoveCarHumanByInput(Car *car, CCar *cCar, COnline *cOnline, CTransformable *cTransformable, CSpeed *cSpeed, CNitro *cNitro, CExternalForce *cExternalForce) {
    bool accDec = false;
    bool turning = false;
    bool skidding = false;
    // actualizar inputs
    for(Constants::InputTypes input : cOnline->inputs){
        if(input == Constants::InputTypes::FORWARD){
            AccelerateHuman(*cCar, *cNitro);
            accDec = true;
        }else if(input == Constants::InputTypes::BACK){
            DecelerateHuman(*cCar, *cNitro);
            accDec = true;
        }else if(input == Constants::InputTypes::LEFT){
            TurnLeftHuman(*cCar);
            turning = true;
        }else if(input == Constants::InputTypes::RIGHT){
            TurnRightHuman(*cCar);
            turning = true;
        }else if(input == Constants::InputTypes::CLAXON){
            // evento
        }else if(input == Constants::InputTypes::LAUNCH_PU){
            shared_ptr<DataMap> data = make_shared<DataMap>();

            (*data)[DataType::ACTUAL_CAR] = car;
            EventManager::GetInstance().AddEventMulti(Event{EventType::THROW_POWERUP_HUMAN, data});
        }else if(input == Constants::InputTypes::DRIFT){
            SkidHuman(*cCar, *cTransformable);
            skidding = true;
        }
    }
    if(accDec == false)
        NotAcceleratingOrDeceleratingHuman(*cCar, *cNitro);
    if(turning == false)
        NotTurningHuman(*cCar);
    if(skidding == false)
        NotSkiddingHuman(*cCar, *cTransformable);


    // actualizar posiciones
    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, cSpeed, cExternalForce, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, cExternalForce, deltaTime);
}

void Physics::UpdateHuman(Car *car, int64_t gameTime) {
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    auto cOnline = static_cast<COnline *>(car->GetComponent(CompType::OnlineComp).get());
    auto cExternalForce = static_cast<CExternalForce *>(car->GetComponent(CompType::CompExternalForce).get());

    MoveCarHumanByInput(car, cCar, cOnline, cTransformable, cSpeed, cNitro, cExternalForce);

    // añadimos que se ha calculado una nueva posición por predicción
    auto cBufferOnline = static_cast<CBufferOnline *>(car->GetComponent(CompType::BufferOnline).get());
    cBufferOnline->InsertNewCalculated(gameTime, cTransformable->position, cTransformable->rotation, cCar->speed, cCar->wheelRotation, cCar->skidDeg, cCar->skidRotation);
}




//Entra cuando se presiona la I
void Physics::AccelerateHuman(CCar &cCar, CNitro &cNitro) const{
    //Aumentamos la velocidad
    if (cNitro.activePowerUp == false) {
        cCar.speed += cCar.acceleration;
        if (cCar.speed > cCar.maxSpeed) {
            cCar.speed -= cCar.acceleration * 4.0;
            if (cCar.speed < cCar.maxSpeed)
                cCar.speed = cCar.maxSpeed;
        }
        // cout << "La current speed es " << cCar.speed << endl;
    } else {
        cCar.speed += cNitro.nitroAcceleration;
        if (cCar.speed > cNitro.nitroMaxSpeed) {
            cCar.speed = cNitro.nitroMaxSpeed;
        }
    }
}

//Entra cuando se presiona la O
void Physics::DecelerateHuman(CCar &cCar, CNitro &cNitro) const{
    //Reducimos la velocidad
    if (cNitro.activePowerUp == false) {
        cCar.speed -= cCar.slowDown;
        if (cCar.speed < cCar.reverseMaxSpeed) {  // no se supere la velocidad marcha atras
            cCar.speed = cCar.reverseMaxSpeed;
        }
    } else {
        cCar.speed += cNitro.nitroAcceleration;
        if (cCar.speed > cNitro.nitroMaxSpeed) {
            cCar.speed = cNitro.nitroMaxSpeed;
        }
    }
}

//Entra cuando se presiona la A
void Physics::TurnLeftHuman(CCar &cCar) const{
    if (cCar.speed >= cCar.maxSpeed*0.15) {
        if (cCar.wheelRotation > -cCar.maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar.wheelRotation -= cCar.incrementWheelRotation;
        }
    } else if (cCar.speed <= -cCar.maxSpeed*0.15) {
        if (cCar.wheelRotation > -cCar.maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar.wheelRotation -= cCar.incrementWheelRotation;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar
        if (cCar.wheelRotation >= cCar.decrementWheelRotation) {
            cCar.wheelRotation -= cCar.decrementWheelRotation;
        } else if (cCar.wheelRotation <= -cCar.decrementWheelRotation) {
            cCar.wheelRotation += cCar.decrementWheelRotation;
        } else {
            cCar.wheelRotation = 0;
        }
    }

    if(cCar.skidState == SkidState::SKID_START && cCar.speed>cCar.maxSpeed*0.6 && duration_cast<milliseconds>(system_clock::now() - cCar.skidStart).count() < cCar.skidActivationTime){
        cCar.skidState = SkidState::SKID_TO_LEFT;
        cCar.skidDeg = cCar.skidDegL;
    }
}

//Entra cuando se presiona la D
void Physics::TurnRightHuman(CCar &cCar) const{
    if (cCar.speed >= cCar.maxSpeed*0.15) {
        if (cCar.wheelRotation < cCar.maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar.wheelRotation += cCar.incrementWheelRotation;
        }
    } else if (cCar.speed <= -cCar.maxSpeed*0.15) {
        if (cCar.wheelRotation < cCar.maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar.wheelRotation += cCar.incrementWheelRotation;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar
        if (cCar.wheelRotation >= cCar.decrementWheelRotation) {
            cCar.wheelRotation -= cCar.decrementWheelRotation;
        } else if (cCar.wheelRotation <= -cCar.decrementWheelRotation) {
            cCar.wheelRotation += cCar.decrementWheelRotation;
        } else {
            cCar.wheelRotation = 0;
        }
    }

    if(cCar.skidState == SkidState::SKID_START && cCar.speed>cCar.maxSpeed*0.6 && duration_cast<milliseconds>(system_clock::now() - cCar.skidStart).count() < cCar.skidActivationTime){
        cCar.skidState = SkidState::SKID_TO_RIGHT;
        cCar.skidDeg = cCar.skidDegR;
    }
}

//Aqui entra cuando no se esta presionando ni I ni O
void Physics::NotAcceleratingOrDeceleratingHuman(CCar &cCar, CNitro &cNitro) const{
    if (cNitro.activePowerUp == false) {
        if (cCar.speed > 0) {
            cCar.speed -= cCar.friction;
            if (cCar.speed < 0)
                cCar.speed = 0;
        } else if (cCar.speed < 0) {
            cCar.speed += cCar.friction;
            if (cCar.speed > 0)
                cCar.speed = 0;
        }
    } else {
        cCar.speed += cNitro.nitroAcceleration;
        if (cCar.speed > cNitro.nitroMaxSpeed) {
            cCar.speed = cNitro.nitroMaxSpeed;
        }
    }
}


//Aqui entra cuando no se esta presionando ni A ni D
void Physics::NotTurningHuman(CCar &cCar) const{
    if (cCar.wheelRotation >= cCar.decrementWheelRotation) {
        cCar.wheelRotation -= cCar.decrementWheelRotation;
    } else if (cCar.wheelRotation <= -cCar.decrementWheelRotation) {
        cCar.wheelRotation += cCar.decrementWheelRotation;
    } else {
        cCar.wheelRotation = 0;
    }
}





void Physics::SkidHuman(CCar &cCar, CTransformable &cTrans) const{
    if(cCar.skidState == SkidState::DISABLED ){ // Reiniciamos el reloj y comenzamos, en caso de pulsar una de las dos direcciones dentro de un tiempo comenzara el giro del derrape
        cCar.skidStart = system_clock::now();
        cCar.skidState = SkidState::SKID_START;
    }

    // Si se mantiene pulsado va a incrementar hasta que se alcance la posicion
    if(cCar.skidState == SkidState::SKID_TO_LEFT){
        cCar.skidRotation -= cCar.skidAcc;
        cTrans.rotation.y -= cCar.skidAcc;
        if(cCar.skidRotation < cCar.skidDeg){
            cCar.skidRotation += cCar.skidAcc;
            cTrans.rotation.y += cCar.skidAcc;
            cCar.skidState = SkidState::SKID_LEFT;
        }
        if(cTrans.rotation.y < 0) cTrans.rotation.y += 360.0;
    }else if(cCar.skidState == SkidState::SKID_TO_RIGHT){
        cCar.skidRotation += cCar.skidAcc;
        cTrans.rotation.y += cCar.skidAcc;
        if(cCar.skidRotation > cCar.skidDeg){
            cCar.skidRotation -= cCar.skidAcc;
            cTrans.rotation.y -= cCar.skidAcc;
            cCar.skidState = SkidState::SKID_RIGHT;
        }
        if(cTrans.rotation.y >= 360) cTrans.rotation.y -= 360.0;  
    }
    if(cCar.speed<cCar.maxSpeed*0.5){
        if(cCar.skidState == SkidState::SKID_LEFT || cCar.skidState == SkidState::SKID_TO_LEFT)
            cCar.skidState = SkidState::SKID_RECOVER_LEFT;
        else if(cCar.skidState == SkidState::SKID_RIGHT || cCar.skidState == SkidState::SKID_TO_RIGHT)
            cCar.skidState = SkidState::SKID_RECOVER_RIGHT;
    }

    RecoverSkid(cCar, cTrans);
}


void Physics::NotSkiddingHuman(CCar &cCar, CTransformable &cTrans) const{
    // En caso de dejar de pulsar el derrape va a tener un periodo de recuperacion del derrape
    if( cCar.skidState == SkidState::SKID_LEFT || cCar.skidState == SkidState::SKID_TO_LEFT)
        cCar.skidState = SkidState::SKID_RECOVER_LEFT;
    else if( cCar.skidState == SkidState::SKID_RIGHT || cCar.skidState == SkidState::SKID_TO_RIGHT)
        cCar.skidState = SkidState::SKID_RECOVER_RIGHT;

    RecoverSkid(cCar, cTrans);
    
    if(cCar.skidState == SkidState::SKID_START && duration_cast<milliseconds>(system_clock::now()-cCar.skidStart).count() > cCar.skidAnimationTime){
        cCar.skidState = SkidState::DISABLED;
        cCar.skidRotation = 0;
    }
}