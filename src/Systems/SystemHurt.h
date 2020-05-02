#pragma once

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Entity;

class SystemHurt {
   public:
    SystemHurt() = default;
    ~SystemHurt() = default;

    void Update(vector<shared_ptr<Entity>> entities);

   private:
};