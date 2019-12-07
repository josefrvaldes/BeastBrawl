#include "Entity.h"

void Entity::AddComponent(shared_ptr<Component> c)
{
    m_components[c->getType()] = c;
}

bool Entity::HasComponent(CompType type)
{
    //return m_components.count(type) > 0;
    return m_components.find(type) != m_components.end();
}