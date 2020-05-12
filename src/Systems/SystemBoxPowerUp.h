#pragma once

#include <Managers/ManBoxPowerUp.h>
#include <Entities/BoxPowerUp.h>

using namespace std;

class SystemBoxPowerUp {

   public:
    SystemBoxPowerUp();
    ~SystemBoxPowerUp();
    void update(ManBoxPowerUp* manBoxPowerUp);

    private:
     const float offsetScale { 0.2 };
     bool increase { false };

};