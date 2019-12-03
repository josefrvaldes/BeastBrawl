#pragma once

#include "../Components/CType.h"
#include "../Components/CId.h"
#include "../Components/CTransformable.h"
#include "../Components/CLastPosition.h"
#include "../Components/CCamera.h"
#include "../Components/Component.h"
#include "../Components/CCar.h"

#include "../Entities/Car.h"

#include "../Managers/ManWayPoint.h"
#include "../Entities/WayPoint.h"

#include "../EventManager/EventManager.h"
#include "../EventManager/Event.h"

#include <vector>

#include <math.h>
using namespace std;
#define PI 3.141592

class PhysicsIA
{
public:
    PhysicsIA();
    ~PhysicsIA();

    void update(vector<WayPoint *> wayPoints, Car* car);

protected:
private:
    uint32_t actualWayPoint = 0;
};