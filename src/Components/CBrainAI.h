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

#include <glm/vec3.hpp>
#include <string>
#include <stack>
#include <vector>
#include <memory>
#include "../Entities/Car.h"
#include "../Entities/Totem.h"
#include "../Entities/BoxPowerUp.h"
#include "../Systems/SystemFuzzyLogicAI.h"


enum class BrainAIDifficult { EASY, NORMAL, DIFFICULT };

class CBrainAI : public Component{
public:
    CBrainAI();
    CBrainAI(BrainAIDifficult dif, float timeTotem);
    ~CBrainAI(){};

    void CleanVisionRange();



    // CTargetNavMesh
    int targetNavMesh {-1};  //NavMesh actual del coche/entidad     //ponemos -1 por defecto ya que haremos el calculo al empezar la partida

    // CPath
    std::stack<int> stackPath;

    // CMovementType
    std::string movementType {"Empty"};     // Para el debug de la IA decirte que decision estas tomando

    // elementos en el rango de vision
    int fov = 50;
    float distanceVision {600};
    std::vector<Entity*> carInVision;
    std::vector<Entity*> boxInVision;
    Entity* totemInVision {nullptr};
    
    // objetivo
    Car* targetCar {nullptr};
    BoxPowerUp* targetBoxPowerUp {nullptr};

    // Pensamiento de la IA = mirar minimapa, mirar marcadores, etc, durante ese tiempo no actua
    bool thinking {false};
    uint32_t totalTried {3};
    uint32_t numTried {2}; // empezamos ya en el ultimo intento porque la primera vez no queremos pensar (al inicio de la partida)
    // Tiempo para ponerse seria, e ir a por el que tiene el totem
    float timeFollowDirect { 15 };

    //Dificultad
    BrainAIDifficult difficult { BrainAIDifficult::NORMAL };

    // LOGICA DIFUSA
    shared_ptr<SystemFuzzyLogicAI> fuzzyLogic;


    // CPosDestinatation
    // glm::vec3 pos_Destinatation = glm::vec3(0.0,0.0,0.0);
    // int id_Destinatation;
    // float radious_Destinatation = 30.0f;
    //int type;
    // tenemos varios tipos:
    // 0 : cuando el destino es un nodo
    // 1 : cuando el destino es un Totem o un coche 


    // CWayPoint
    // glm::vec3 position_WayPoint = glm::vec3(0.0,0.0,0.0);
    // int type_WayPoint;
    // int id_WayPoint;
    // float radious_WayPoint = 30.0f;


};