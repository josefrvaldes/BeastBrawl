#pragma once

#include "Component.h"

#include "../include/glm/vec3.hpp"
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;
//using namespace glm;
enum class typeCPowerUp {
    None,               // 0
    RoboJorobo,         // 1
    SuperMegaNitro,     // 2
    PudinDeFrambuesa,   // 3
    EscudoMerluzo,      // 4
    TeleBanana,         // 5
    MelonMolon          // 6
};

class CPowerUp : public Component{
public:
    CPowerUp();
    CPowerUp(typeCPowerUp _typePowerUp);
    //CPowerUp(glm::vec3 _pos, int type,int id);
    ~CPowerUp();

    // friend ostream &operator<<(ostream &out, CWayPoint &c)
    // {
    //     //cout << "(" << c.position.x << "," << c.position.y << "," << c.position.z << ")";
    //     //return out;
    // }
    typeCPowerUp typePowerUp = typeCPowerUp::None;   // NONE

    // Tiempo de inactividad mientras sale el powerUp hasta que hace efecto
    bool effectActive = false;
    bool calculate = false;
    time_point<system_clock> timeStart;
    const int64_t durationTime = 100;       // 0,2 segundos ---> Podra variar dependiendo de cada powerUp
    float speed = 510.0;
    float deceleration = -25;
};