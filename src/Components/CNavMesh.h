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

#include "Component.h"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class CNavMesh : public Component
{
public:
    CNavMesh(vector<int> _waypoints);
    CNavMesh();
    ~CNavMesh(){};

    void ResetNumIds(){numIdsNavMesh = 0;}

    uint16_t id; //Id del navmesh
    vector<int> waypoints; // Waypoints que tiene el navmesh
    bool centralNavMESH {false}; //indica si es el navMehs central

private:
    static uint16_t numIdsNavMesh;  //Lleva la cuenta del id de navMesh
};
