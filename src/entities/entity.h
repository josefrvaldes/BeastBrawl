#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "../components/position.h"
#include <iostream>
#include <unordered_map>

using namespace std;

class Entity
{

public:
    Entity(){};
    ~Entity(){};

    unordered_map<CompType, Component> getComponents() { return m_components; };
    void AddComponent(Component c);

protected:
    unordered_map<CompType, Component> m_components;

private:
};

#endif