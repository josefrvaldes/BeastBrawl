#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "../Components/Component.h"
#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;

class Entity{

public:
    Entity(){};
    ~Entity(){};

    unordered_map<CompType, shared_ptr<Component>> GetComponents() { return m_components; };
    void AddComponent(shared_ptr<Component> c);
    bool HasComponent(CompType type);

protected:
    unordered_map<CompType, shared_ptr<Component>> m_components;

private:
};

#endif