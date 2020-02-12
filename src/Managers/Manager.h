#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <memory>

using namespace std;

class Entity;

class Manager {
   public:
    Manager();
    ~Manager(); 

    const vector<shared_ptr<Entity>>& GetEntities() const {
        return entities;
    };

    
    virtual void Integrate(float) {};

   protected:
    vector<shared_ptr<Entity>> entities;
    virtual void SubscribeToEvents() {};
};
