#include "entity.h"

void Entity::AddComponent(Component c)
{
    m_components[c.getType()] = c;
}