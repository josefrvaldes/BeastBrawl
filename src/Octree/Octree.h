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
#include "glm/vec3.hpp"
#include "../Entities/Entity.h"


class Manager;
class ManCar;
class ManPowerUp;
class RenderFacade;

enum TypeCollision{ Inside, Border, No_Collision}; 
struct BoundingOctree{
    float sizeAABB;
    glm::vec3 centerAABB;
};


class Octree{
  public:
    Octree(const glm::vec3& posCenter, float _size, const std::vector<shared_ptr<Manager>>& vecManagers);
    Octree(const glm::vec3& posCenter, float _size, const std::vector<std::shared_ptr<Entity>>& vecEntities);
    ~Octree(){};

    void CreateTree();
    void SetFather(Octree* _father){father = _father; };
    void Draw(RenderFacade* renderEngine) const;
    void UpdateVisibleObjects(RenderFacade* renderEngine);


  private:
    // void AddChild();
    TypeCollision CollideAABB(Entity* object, const BoundingOctree& nodeBox) const;
    void DrawCube(RenderFacade* renderEngine, int r, int g, int b) const;

    glm::vec3 centralPosition;
    float size;                 // es el radio o la mitad del cuadrado
    const float minSize = 15.0;  // es el radio o la mitad del cuadrado

    Octree* father {nullptr};
    
    std::vector<std::shared_ptr<Entity>> entitiesContained;

    unsigned int numChilds = 0;
    std::unique_ptr<Octree> childs[8];

    bool visible = false;

};