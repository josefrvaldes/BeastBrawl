#pragma once

#include <memory>
#include <fuzzyLogic/fuzzyLogic.h>

class CarAI;
class FuzzyLogic;

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
};