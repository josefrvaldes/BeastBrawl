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


#include <vector>
#include <stack>
#include <iostream>
#include <cstdio>
#include <climits>
#include <algorithm>
#include <cstdlib>     /* srand, rand */

#include <cmath>
#include <memory>
using namespace std;

struct CarAI;
struct Entity;
struct ManWayPoint;

class PhysicsAI {
public:
    PhysicsAI();
    ~PhysicsAI(){};
/*
    void Update(CarAI* carAI, ManWayPoint* graph);
    void UpdateCarPowerUps(Entity* carAI);
*/
};