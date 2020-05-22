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
 * @author Carlos de la Fuente Torres <delafuentetorresc@gmail.com>
 * @author Antonio Jose Martinez Garcia
 * 
 */
 
 
 #include "SystemFuzzyLogicAI.h"

#include <Components/CTransformable.h>
#include <Components/CExternalForce.h>
#include <Components/Component.h>
#include <Components/CCar.h>
#include <Components/CWayPoint.h>
#include <Components/CNitro.h>
#include <Entities/CarAI.h>
#include "../Systems/Utils.h"


#include <cmath>
#include <iostream>
#include <memory>
#define PI 3.141592


void CalculatePositionAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle);
void CalculatePositionReverseAI(CCar* cCar, CTransformable* cTransformable, float deltaTime, float angle);


SystemFuzzyLogicAI::SystemFuzzyLogicAI(){

}


void SystemFuzzyLogicAI::InitSystemFuzzyLogicAI(CarAI* car){
    fuzzyRules(car);
}


// inicializa las reglas de la logica difusa
void SystemFuzzyLogicAI::fuzzyRules(CarAI* car){
    auto cCar        = static_cast<CCar*>(car->GetComponent(CompType::CarComp).get());
    float maxSpeed = cCar->maxSpeed;
    float minSpeed = cCar->reverseMaxSpeed;
    float accelerationCar = cCar->acceleration;
    //float algo = cCar->
    fuzzyRulesVelocity(maxSpeed, minSpeed, accelerationCar);
    fuzzyRulesAngle();
}


void SystemFuzzyLogicAI::fuzzyRulesVelocity(float maxSpeed, float minSpeed, float accelerationCar){

    shared_ptr<FuzzyVariable> ActualVelocity = flVelocity->CreateFLV("ActualVelocity");
    shared_ptr<FzSet> Velocity_Slow = ActualVelocity->AddLeftShoulderSet("Velocity_Slow", minSpeed, 0, 50);
    shared_ptr<FzSet> Velocity_Normal = ActualVelocity->AddTriangularSet("Velocity_Normal", 0, 50, maxSpeed/2);
    shared_ptr<FzSet> Velocity_High = ActualVelocity->AddRightShoulderSet("Velocity_High", 50, maxSpeed/2, maxSpeed);

    shared_ptr<FuzzyVariable> Angle = flVelocity->CreateFLV("Angle");
    shared_ptr<FzSet> Angle_Slow = Angle->AddLeftShoulderSet("Angle_Slow", 0, 30, 60);
    shared_ptr<FzSet> Angle_Normal = Angle->AddTriangularSet("Angle_Normal", 30, 60, 120);
    shared_ptr<FzSet> Angle_High = Angle->AddRightShoulderSet("Angle_High", 60, 120, 180);

    //FuzzyVariable& Distance = flVelocity.CreateFLV("Distance");
    //FzSet Distance_Near = Distance.AddLeftShoulderSet("Distance_Near", 0, 0, 10);
    //FzSet Distance_nNormal = Distance.AddTriangularSet("Distance_nNormal", 0, 10, 30);
    //FzSet Distance_Far = Distance.AddRightShoulderSet("Distance_Far", 10, 30, 40);

    shared_ptr<FuzzyVariable> Acceleration = flVelocity->CreateFLV("Acceleration");
    shared_ptr<FzSet> Accelerate_Brake = Acceleration->AddLeftShoulderSet("Accelerate_Brake", -accelerationCar, -accelerationCar/2, 0);
    shared_ptr<FzSet> Accelerate_None = Acceleration->AddTriangularSet("Accelerate_None", -accelerationCar/2, 0,accelerationCar/2);
    shared_ptr<FzSet> Accelerate_Max = Acceleration->AddRightShoulderSet("Accelerate_Max", 0, accelerationCar/2, accelerationCar);
    // To-Do: revisar el new por que no se tiene que hacer
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Slow, Angle_Slow)), Accelerate_Max);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Slow, Angle_Normal)), Accelerate_Max);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Slow, Angle_High)), Accelerate_Max);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Normal, Angle_Slow)), Accelerate_Max);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Normal, Angle_Normal)), Accelerate_None);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_Normal, Angle_High)), Accelerate_None);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_High, Angle_Slow)), Accelerate_Max);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_High, Angle_Normal)), Accelerate_Brake);
    flVelocity->AddRule( (make_shared<FzAND>(Velocity_High, Angle_High)), Accelerate_Brake);
}

void SystemFuzzyLogicAI::fuzzyRulesAngle(){

    shared_ptr<FuzzyVariable> Distance = flAngle->CreateFLV("Distance");
    shared_ptr<FzSet> Distance_Near = Distance->AddLeftShoulderSet("Distance_Near", 0, 0, 40);
    shared_ptr<FzSet> Distance_Normal = Distance->AddTriangularSet("Distance_Normal", 20, 40, 60);
    shared_ptr<FzSet> Distance_Far = Distance->AddRightShoulderSet("Distance_Far", 40, 100, 100);

    shared_ptr<FuzzyVariable> Direction = flAngle->CreateFLV("Direction");
    shared_ptr<FzSet> Direction_Left = Direction->AddLeftShoulderSet("Direction_Left", -180, -180, 0);
    shared_ptr<FzSet> Direction_Center = Direction->AddTriangularSet("Direction_Center", -20, 0, 20);
    shared_ptr<FzSet> Direction_Right = Direction->AddRightShoulderSet("Direction_Right", 0, 180, 180);

    shared_ptr<FuzzyVariable> Rotation = flAngle->CreateFLV("Rotation");
    shared_ptr<FzSet> Rotation_Left = Rotation->AddLeftShoulderSet("Rotation_Left", -10, -5, 0);
    shared_ptr<FzSet> Rotation_None = Rotation->AddTriangularSet("Rotation_None", -5, 0, 5);
    shared_ptr<FzSet> Rotation_Right = Rotation->AddRightShoulderSet("Rotation_Right", 0, 5, 10);

// TODO
// SI VA A COLISIONAR ----- HARA LA ROTACION HARD... QUE SERÁ DERRAPAR  

/*
    shared_ptr<FuzzyVariable> Rotation = flAngle->CreateFLV("Rotation");
    shared_ptr<FzSet> Rotation_Hard_Left = Rotation->AddLeftShoulderSet("Rotation_Hard_Left", -10, -10, -5);
    shared_ptr<FzSet> Rotation_Left = Rotation->AddTriangularSet("Rotation_Left", -10, -5, 0);
    shared_ptr<FzSet> Rotation_None = Rotation->AddTriangularSet("Rotation_None", -5, 0, 5);
    shared_ptr<FzSet> Rotation_Right = Rotation->AddTriangularSet("Rotation_Right", 0, 5, 10);
    shared_ptr<FzSet> Rotation_Hard_Right = Rotation->AddRightShoulderSet("Rotation_Hard_Right", 5, 10, 10);
*/

    // To-Do: revisar el new por que no se tiene que hacer
    flAngle->AddRule( (make_shared<FzAND>(Distance_Near, Direction_Left)), Rotation_Left);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Near, Direction_Center)), Rotation_None);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Near, Direction_Right)), Rotation_Right);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Normal, Direction_Left)), Rotation_Left);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Normal, Direction_Center)), Rotation_None);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Normal, Direction_Right)), Rotation_Right);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Far, Direction_Left)), Rotation_Left);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Far, Direction_Center)), Rotation_None);
    flAngle->AddRule( (make_shared<FzAND>(Distance_Far, Direction_Right)), Rotation_Right);
}




//Nos devuelve el angulo en radianos entre el coche y el punto destino del coche
float calculateAngle(CPosDestination* posDestination, CarAI* car,CCar* cCar){
    auto cTransformable = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());

    // calcular vector al wayPoint
    float vetorWaypointX = (posDestination->position.x - cTransformable->position.x );
    float vetorWaypointZ = (posDestination->position.z - cTransformable->position.z);
    
    // se calcula el siguiente punto al que avanzara el coche
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float posXSiguiente = cTransformable->position.x - cos(angleRotation) * cCar->speed;
    float posZSiguiente = cTransformable->position.z + sin(angleRotation) * cCar->speed;
    // se calcula el vector entre el siguiente punto y y el punto actual del coche
    float xCoche = (posXSiguiente - cTransformable->position.x );
    float zCoche = (posZSiguiente - cTransformable->position.z);

    // se calcula el angulo entre los dos vectores
    float numerador = xCoche*vetorWaypointX + zCoche*vetorWaypointZ;
    float denominador = sqrt(xCoche*xCoche + zCoche*zCoche) * sqrt(vetorWaypointX*vetorWaypointX + vetorWaypointZ*vetorWaypointZ);
    float angle2 = 0.0;
    if(denominador!=0)
        angle2 = acos(numerador/denominador);
    angle2 = (angle2 * 180) / PI;

    // calcular si tiene que girar a la izquierda o derecha
    float valueAtan2 = atan2(vetorWaypointZ,vetorWaypointX)*180/PI;
    valueAtan2 = 180.0 - valueAtan2; // se le restan ya que el eje empieza en el lado contrario 
    if(valueAtan2<0)
        valueAtan2 += 360;

    ////calcular opuesto
    float oppositeAngleAtan2 = 0.0;
    if(valueAtan2<180)
        oppositeAngleAtan2 = valueAtan2 + 180;
    else
        oppositeAngleAtan2 = valueAtan2 - 180;
    
    // sentido de giro
    float prueba = 0.0;
    if(valueAtan2>=180){ // este if se hace para comprobar que se pueda hacer el && y no caiga en la parte que se reinician los grados
        if(cTransformable->rotation.y>oppositeAngleAtan2 && cTransformable->rotation.y<valueAtan2)
            prueba = angle2;
        else
            prueba = angle2 * (-1);
    }else{
        if(cTransformable->rotation.y>valueAtan2 && cTransformable->rotation.y<oppositeAngleAtan2)
            prueba = angle2 * (-1);
        else
            prueba = angle2;
    }

    return prueba;
}


float SystemFuzzyLogicAI::calculateFuzzyVelocity(float speedCar, float angle){
    flVelocity->Fuzzify("ActualVelocity", speedCar); // AQUI ES DONDE SE LLAMA AL CALCULATEDOM()
    flVelocity->Fuzzify("Angle", angle);

    float defuzzificacion = flVelocity->DeFuzzify("Acceleration"); 
    
    return defuzzificacion;
}


float SystemFuzzyLogicAI::calculateFuzzyDirection(float distance, float direction){
    flAngle->Fuzzify("Distance", distance); // AQUI ES DONDE SE LLAMA AL CALCULATEDOM()
    flAngle->Fuzzify("Direction", direction);
    float defuzzificacion = flAngle->DeFuzzify("Rotation"); 
    
    return defuzzificacion;
}





void SystemFuzzyLogicAI::Update(CarAI* car, float deltaTime){
    //Guardamos en varAIbles los componentes
	auto cTransformable = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());
    //auto cWayPoint     = static_cast<CWayPoint*>(car->GetComponent(CompType::WayPointComp).get());
    auto cPosDestination = static_cast<CPosDestination*>(car->GetComponent(CompType::PosDestination).get());
    auto cExternalForce = static_cast<CExternalForce*>(car->GetComponent(CompType::CompExternalForce).get());
    auto cCar        = static_cast<CCar*>(car->GetComponent(CompType::CarComp).get());
    float angleRange = 0;
    float angle = 0;
    //float distance2P = sqrt( pow((cWayPoint->position.x - cTransformable->position.x),2) + pow((cWayPoint->position.z - cTransformable->position.z),2) );
    float distance2P = sqrt( pow((cPosDestination->position.x - cTransformable->position.x),2) + pow((cPosDestination->position.z - cTransformable->position.z),2) );


    // LOGICA DIFUSA:
    angle = calculateAngle(cPosDestination, car, cCar);
    if (angle < 0)
        angleRange = angle*(-1);
    else
        angleRange = angle;

    //cout << "Angulo Positivo: " << angleRange*180/PI << endl;
    if(cCar->speed == 0)
        cCar->speed = 1.0;
        //std::cout << "VOY A ENTRAR A VELOCITY DIFUSA" <<std::endl;
    float fuzzyAceleration = calculateFuzzyVelocity(cCar->speed, angleRange);
    float fuzzyRotation = 0.0;
    if(cCar->speed>30 || cCar->speed < -30){
        fuzzyRotation = calculateFuzzyDirection(distance2P, angle);
    }

    //Aumentamos la velocidad
    //cCar->wheelRotation = angle
    auto cNitro2 = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());

    cCar->wheelRotation = fuzzyRotation;
    if(cNitro2->activePowerUp == false){
        cCar->speed += fuzzyAceleration;
        if (cCar->speed > cCar->maxSpeed) {
            cCar->speed -= cCar->acceleration*4.0;
            if(cCar->speed < cCar->maxSpeed)
                cCar->speed = cCar->maxSpeed;
        }
    }else{
        cCar->speed += cNitro2->nitroAcceleration;
        if(cCar->speed > cNitro2->nitroMaxSpeed){
            cCar->speed = cNitro2->nitroMaxSpeed;
        }
    }


    // calculamos las posiciones
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    glm::vec2 carForce(-cos(angleRotation)*cCar->speed , sin(angleRotation)*cCar->speed);  // Fijarse en el menos de la X
    glm::vec2 finalForce = ApplyExternalForce(cCar, cExternalForce, carForce);

    cTransformable->position.x += finalForce.x * deltaTime;
    cTransformable->position.z += finalForce.y * deltaTime;
    if(cCar->wheelRotation != 0){
        cTransformable->rotation.y += cCar->wheelRotation * 0.20;
        cTransformable->rotation.y = Utils::GetAdjustedDegrees(cTransformable->rotation.y);
    }
}



// aplicamos al movimiento del coche el desplazamiento en caso de que alguien lo empuje
glm::vec2 SystemFuzzyLogicAI::ApplyExternalForce(CCar *cCar, CExternalForce *externalForce, const glm::vec2& carForce) const{
    glm::vec2 finalForce(carForce);
    if(externalForce->force > 0){
        glm::vec2 collisionForce(externalForce->dirExternalForce.x*externalForce->force, externalForce->dirExternalForce.z*externalForce->force);
        //float angleForces = glm::degrees(atan2(collisionForce.y, collisionForce.x)) - glm::degrees(atan2(carForce.y, carForce.x));
        //angleForces = Utils::GetAdjustedDegrees(angleForces);
        finalForce.x = carForce.x + collisionForce.x;
        finalForce.y = carForce.y + collisionForce.y;
        externalForce->force -= externalForce->friction;
    }else{
        externalForce->force = 0.0;
    }
    return finalForce;
}