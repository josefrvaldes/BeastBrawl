#include "Physics.h"

void Physics::move(CSpeed *s, CPosition *p)
{
    p->SetX(p->GetX() + s->GetVX());
    p->SetY(p->GetY() + s->GetVY());
    p->SetZ(p->GetZ() + s->GetVZ());
}