#ifndef __MANAGER__
#define __MANAGER__

#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Entity;

class Manager {
   public:
    Manager() = default;
    virtual ~Manager() {
        cout << "Se llama al destructor de manager" << endl;
        entities.clear();
        entities.shrink_to_fit();
    }

    vector<shared_ptr<Entity>> GetEntities() const {
        return entities;
    };

   protected:
    vector<shared_ptr<Entity>> entities;
    virtual void SubscribeToEvents() = 0;
};

#endif