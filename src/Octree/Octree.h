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
    void DrawCube(RenderFacade* renderEngine) const;

    glm::vec3 centralPosition;
    float size;                 // es el radio o la mitad del cuadrado
    const float minSize = 200.0;  // es el radio o la mitad del cuadrado

    Octree* father {nullptr};
    
    std::vector<std::shared_ptr<Entity>> entitiesContained;

    int numChilds = 0;
    std::unique_ptr<Octree> childs[8];

    bool visible = false;

};