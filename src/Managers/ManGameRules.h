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

#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <Entities/GameObject.h>
#include <GameValues.h>

using namespace std;

class Entity;
struct SystemGameRules;

class ManGameRules{
   public:
    ManGameRules();
    ~ManGameRules();

    const unordered_map<uint16_t, glm::vec2>& GetPositionsPlane()   { return positionsPlane; };
    const glm::vec2& GetPositionTotemPlane()                         { return positionTotemPlane; };

    void InitializeMiniMap(const vector<shared_ptr<Entity>> &cars, const vector<shared_ptr<Entity>> &totems);

    void SubscribeToEvents();
    void RestartAllTimers(vector<shared_ptr<Entity>> entities, int64_t timeStartPause);
    void ResetClock();

    void CreateCurrentMap(MAP type);
    vector<shared_ptr<GameObject>> GetGrounds(){ return ground; }; 
    /**
     * @return true o false dependiendo si la partida debe terminar o no
     */
    bool Update(const vector<shared_ptr<Entity>> &cars, const vector<shared_ptr<Entity>> &totems);
    unique_ptr<Entity>& GetGlobalClock() { return globalClock; };

    glm::vec3 GetPosTotem(){return posTotem; }; 

   private:
    void CreateMayanMap();
    void CreateGlobalClock( const uint32_t durationTime_ );
    unique_ptr<Entity> globalClock;
    unique_ptr<SystemGameRules> systemGameRules;
    glm::vec3 posTotem;

    unordered_map<uint16_t, glm::vec2> positionsPlane;
    glm::vec2 positionTotemPlane { glm::vec2(-1, -1) };


    vector<shared_ptr<GameObject>> ground;

};
