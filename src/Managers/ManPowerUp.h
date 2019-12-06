#ifndef __MAN_POWERUP_H__
#define __MAN_POWERUP_H__

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Manager.h"

using namespace std;

class ManPowerUp : public Manager {
   public:
    ManPowerUp();
    ~ManPowerUp();

    void CreatePowerUp(float x, float y, float z);
    void CreatePowerUp();

   private:
    void SubscribeToEvents();
};

#endif