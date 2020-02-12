#pragma once

#include <Components/Component.h>
#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;

class Entity{

public:
    Entity(){};
    ~Entity(){};

    unordered_map<CompType, shared_ptr<Component>> GetComponents() { return m_components; };
    shared_ptr<Component> GetComponent(CompType type);
    void AddComponent(const shared_ptr<Component>& c);

    bool HasComponent(CompType type);

protected:
    unordered_map<CompType, shared_ptr<Component>> m_components;

private:
};
