#pragma once

#include "../../lib/glm/vec3.hpp"

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;
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
    //CPowerUp(glm::vec3 _pos, int type,int id);
    ~CPowerUp();

    // friend ostream &operator<<(ostream &out, CWayPoint &c)
    // {
    //     //cout << "(" << c.position.x << "," << c.position.y << "," << c.position.z << ")";
    //     //return out;
    // }
    typeCPowerUp typePowerUp = typeCPowerUp::None;   // NONE
};