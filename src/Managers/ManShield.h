/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
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