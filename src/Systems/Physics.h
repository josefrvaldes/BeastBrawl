#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "../Components/CSpeed.h"
#include "../Components/CPosition.h"

class Physics
{
public:
    Physics(){};
    ~Physics(){};
    static void move(CSpeed *s, CPosition *p);

protected:
private:
};

#endif