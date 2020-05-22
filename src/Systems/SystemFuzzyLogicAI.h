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
 
 
 #pragma once

#include <memory>
#include <fuzzyLogic/fuzzyLogic.h>
#include "../../include/glm/glm.hpp"


class CarAI;
class FuzzyLogic;
class CCar;
class CExternalForce;

using namespace std;

class SystemFuzzyLogicAI {
public:
    SystemFuzzyLogicAI();
    ~SystemFuzzyLogicAI(){};

    void InitSystemFuzzyLogicAI(CarAI* car);
    void Update(CarAI* car, float deltaTime);
    void fuzzyRules(CarAI* car);
    void fuzzyRulesVelocity(float maxSpeed, float minSpeed, float accelerationCar);
    void fuzzyRulesAngle();
    float calculateFuzzyVelocity(float speedCar, float angle);
    float calculateFuzzyDirection(float distance, float direction);


protected:
private:
    uint32_t actualWayPoint = 0;
    shared_ptr<FuzzyLogic> flVelocity = make_shared<FuzzyLogic>();
    shared_ptr<FuzzyLogic> flAngle = make_shared<FuzzyLogic>();
    
    glm::vec2 ApplyExternalForce(CCar *cCar, CExternalForce *externalForce, const glm::vec2& carForce) const;

};