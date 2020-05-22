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

#include "ManCar.h"

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <Facade/Render/RenderFacadeManager.h>
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>


class NavMesh;
struct ManTotem;

using namespace std;
struct Car;
struct CarAI;
struct Data;
struct CTransformable;
struct ManCar;

class ManNavMesh : public Manager{
   public:
    //ManNavMesh(){};
    ManNavMesh();
    ~ManNavMesh(){};

    void CreateNavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, vector<int> waypoints);
    void CreateNavMesh();

    void Update(ManCar &);
    void UpdateNavMeshEntity(Entity* entity_) const;
    //static ManNavMesh& getInstance() {
    //    static ManNavMesh instance;
    //    return instance;
    //};

    //void UpdateNavMeshCarsAI(ManCar* manCars);

    // vector<shared_ptr<NavMesh>> GetEntities() const {
    //     return navMeshes;
    // };

    // vector<shared_ptr<NavMesh>> GetEntities()  {
    //     return navMeshes;
    // };
    int CalculateNavMesh(glm::vec3 &) const;    // version para llamar desde fuera de la clase NavMesh

   private:
    //vector<shared_ptr<NavMesh>> navMeshes;
    void SubscribeToEvents() override;
    void ActualizeNavMeshTotem(DataMap* d);
    void InitNavMeshTotem(ManTotem *manTotems);
    void ActualizeNavMeshCarAI(DataMap* d);
    int CalculateNavMesh(CTransformable &) const;
    //void ActualizeNavMeshCarPlayer(Car* carPlayer);
};