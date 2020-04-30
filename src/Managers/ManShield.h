#pragma once

#include "Manager.h"
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <Aliases.h>
#include <Entities/Shield.h>

using namespace std;


class ManShield : public Manager{
    public:
        //using ManNavMesh::CalculateNavMesh;
        ManShield();
        ~ManShield();

        void CreateShield(uint16_t parentId, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        void UpdateShieldVisibility(DataMap* d);
    

    private:
        void SubscribeToEvents() override;

};