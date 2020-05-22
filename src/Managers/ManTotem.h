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

#include "ManNavMesh.h"

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <Aliases.h>
#include "../Systems/SystemOnline.h"
//#include "ManNavMesh.h"

using namespace std;
#define PI 3.141592

class Totem;
class Data;
class ManNavMesh;
class PhysicsTotem;

class ManTotem : public Manager{
   public:
    //using ManNavMesh::CalculateNavMesh;
    ManTotem(ManNavMesh *);
    ~ManTotem();

    void CreateTotem();
    void CreateTotem(glm::vec3 _position);
    void Update();
    void SetSystemOnline(SystemOnline* systOn){ systemOnline = systOn; };
    //shared_ptr<Totem>& GetTotem() { return totem; };
    //vector<shared_ptr<Totem>> GetEntities() const { return totems; };

   private:
    //shared_ptr<Totem> totem;
    //vector<shared_ptr<Totem>> totems;
    void ActivateTotem(const glm::vec3 &position, int currentNavMesh);
    void AppertainCar(DataMap* d);
    void ResetTotem(DataMap* d);
    void SyncTotem(DataMap* d);
    void RecievedCatchTotem(DataMap* d);
    void RecievedLostTotem(DataMap* d);
    void SubscribeToEvents();

    void UpdatePhysics();

    ManNavMesh* manNavMesh;
    SystemOnline* systemOnline = nullptr; // en caso de que sea Single va a ser un nullptr
    unique_ptr<PhysicsTotem> physicsTotem;
};