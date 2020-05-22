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
#include <unordered_map>
#include <glm/glm.hpp>

#include <cmath>
#include <memory>
using namespace std;

struct CarAI;
struct Entity;
struct ManWayPoint;

class SystemGameRules {
    public:
        SystemGameRules();
        SystemGameRules(float, float, float, float);
        ~SystemGameRules(){};

        /**
         * @return true o false dependiendo si la partida debe terminar o no
         */
        bool UpdateRulesCarPowerUps(Entity& car_, Entity& totem_) const;
        
        /**
         * @return true o false dependiendo si la partida debe terminar o no
         */
        bool UpdateGameRules(Entity& globalClock_) const;

        void UpdatePositionsMiniMap(const vector<shared_ptr<Entity>> &cars, const vector<shared_ptr<Entity>> &totems, unordered_map<uint16_t, glm::vec2> &positions, glm::vec2 &positionTotem);
        
        void RestartAllTimers(vector<shared_ptr<Entity>> entities, Entity &globalClock, int64_t timeStartPause);

    private:

        float xLeftMap { 0 };       float xRightMap { 0 };
        float yUpMap { 0 };         float yDownMap { 0 };

        float widthMap { 0 };       float heightMap { 0 };

};