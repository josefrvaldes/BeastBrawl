#include "Physics.h"

void Physics::move(CSpeed *s, CPosition *p)
{
    p->x += s->x;
    p->y += s->y;
    p->z += s->z;
}