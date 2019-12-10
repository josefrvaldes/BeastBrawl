#pragma once

#include "Entity.h"
#include "../../lib/glm/vec3.hpp"
#include <iostream>
#include <map>

using namespace std;

enum class type {
    None,               // 0
    RoboJorobo,         // 1
    SuperMegaNitro,     // 2
    PudinDeFrambuesa,   // 3
    EscudoMerluzo,      // 4
    TeleBanana,         // 5
    MelonMolon          // 6
};

class PowerUp : public Entity{

public:

    PowerUp();
    PowerUp(glm::vec3 _position);
    ~PowerUp();

    type getTypePowerUp(){ return typePowerUp; }

private:
    type typePowerUp; 
};
