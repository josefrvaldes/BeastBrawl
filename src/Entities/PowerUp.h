#pragma once

#include "Entity.h"
#include "../../lib/glm/vec3.hpp"
#include <iostream>
#include <map>

using namespace std;


enum class type {
    roboJorobo,         // 1
    SuperMegaNitro,     // 2
    PudinDeFrambuesa,   // 3
    EscudoMerluzo,      // 4
    TeleBanana,         // 5
    MelonMolon          // 6
};

class PowerUp : public Entity{

public:
    PowerUp();
    PowerUp(glm::vec3 _position, type _typePowerUp);
    ~PowerUp();


private:
    type typePowerUp; 
};
