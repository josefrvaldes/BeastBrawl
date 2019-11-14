#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "../components/speed.h"
#include "../components/position.h"

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