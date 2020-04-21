#include "Octree.h"

#include "../Managers/Manager.h"
#include "../Managers/ManCar.h"
#include "../Managers/ManPowerUp.h"
#include "../Entities/PowerUp.h"
#include "../Entities/PowerUp.h"
#include "../Components/CTransformable.h"
#include "../Components/CDimensions.h"
#include "../Components/CId.h"
#include "../Facade/Render/RenderFacade.h"





Octree::Octree(const glm::vec3& posCenter, float _size, const std::vector<shared_ptr<Manager>>& vecManagers) : centralPosition(posCenter), size(_size){
    // almacenamos en un vector todas las entidades para que sea mas facil utilizarlo
    for(const auto& actualManager : vecManagers){
        for(const auto& actualEntity : actualManager->GetEntities()){
            entitiesContained.emplace_back(actualEntity);
        }
    }

    numChilds = 0;
    //cout << "Empieza a crearse el arbol con " << entitiesContained.size() << " elementos\n";
    CreateTree();
}


Octree::Octree(const glm::vec3& posCenter, float _size, const std::vector<std::shared_ptr<Entity>>& vecEntities) 
    : centralPosition(posCenter), size(_size), entitiesContained(vecEntities){
        numChilds = 0;
}



// funcion recursiva que construye el octree
void Octree::CreateTree(){
    if(entitiesContained.size() == 0)
        return;
    
    float actualSize = size / 2.0;          // es el radio

    if(actualSize < minSize)
        return;


    BoundingOctree boxes[8];
    boxes[0] = {actualSize, centralPosition - glm::vec3(actualSize, actualSize, actualSize)};
    boxes[1] = {actualSize, centralPosition - glm::vec3(actualSize, actualSize, -actualSize)};
    boxes[2] = {actualSize, centralPosition - glm::vec3(actualSize, -actualSize, actualSize)};
    boxes[3] = {actualSize, centralPosition - glm::vec3(actualSize, -actualSize, -actualSize)};
    boxes[4] = {actualSize, centralPosition - glm::vec3(-actualSize, actualSize, actualSize)};
    boxes[5] = {actualSize, centralPosition - glm::vec3(-actualSize, actualSize, -actualSize)};
    boxes[6] = {actualSize, centralPosition - glm::vec3(-actualSize, -actualSize, actualSize)};
    boxes[7] = {actualSize, centralPosition - glm::vec3(-actualSize, -actualSize, -actualSize)};

    std::vector<std::shared_ptr<Entity>> entitiesIn[8]; // para tener las entidades que van a estar dentro
    std::vector<std::shared_ptr<Entity>> entitiesDelete; // para tener las entidades que van a estar dentro


    for(const auto& actualEntity : entitiesContained){
        for(unsigned int i = 0; i < 8; i++){
            TypeCollision typeCol = CollideAABB(actualEntity.get(), boxes[i]);
            if(typeCol == TypeCollision::No_Collision)
                continue;
            
            if(typeCol == TypeCollision::Border)
                break;
            
            if(typeCol == TypeCollision::Inside){
                entitiesIn[i].emplace_back(actualEntity);
                entitiesDelete.emplace_back(actualEntity);
                //cout << "Se anyade un nuevo hijo al array de " << i << "\n";
                break;
            }
        }
    }

    // delete objects from entitiesDlete in entitiesContained   PUEDE FALLAR
    for(const auto& actualDelEntity : entitiesDelete){
        for(auto it=entitiesContained.begin(); it!=entitiesContained.end(); it++){
            if(it->get() == actualDelEntity.get()){
                entitiesContained.erase(it);
                break;
            }
        }
    }

    // Crea los nuevos hijos y vuelve a llamar para construir sus arboles
    for(unsigned int i = 0; i < 8; i++){
        if(entitiesIn[i].size() > 0){
            //cout << "Se crea un nuevo hijo y se construye su octree " << i << "\n";
            childs[numChilds] = make_unique<Octree>(boxes[i].centerAABB ,boxes[i].sizeAABB , entitiesIn[i]);
            childs[numChilds]->SetFather(this);
            childs[numChilds]->CreateTree();
            numChilds++;
        }
    }


}



// calcula si se encuentra dentro del AABB, fuera, o colisiona con el borde
TypeCollision Octree::CollideAABB(Entity* object, const BoundingOctree& nodeBox) const{
    auto cTran = static_cast<CTransformable*>(object->GetComponent(CompType::TransformableComp).get());
    if(!object->HasComponent(CompType::DimensionsComp)){
        //cout << "No existe el CDimensions \n";
        return TypeCollision::Border;   // no tienen el componente
    }

    auto cDim = static_cast<CDimensions*>(object->GetComponent(CompType::DimensionsComp).get());
    float dimension = cTran->scale.x * cDim->boundingBoxMesh * 0.5;
    glm::vec3 nodeMin = nodeBox.centerAABB - nodeBox.sizeAABB;
    glm::vec3 nodeMax = nodeBox.centerAABB + nodeBox.sizeAABB;

    glm::vec3 entMin = glm::vec3(cTran->position.x-dimension, cTran->position.y-dimension, cTran->position.z-dimension);
    glm::vec3 entMax = glm::vec3(cTran->position.x+dimension, cTran->position.y+dimension, cTran->position.z+dimension); 

    // calculate AABB collision
    if((nodeMin.x <= entMax.x && nodeMax.x >= entMin.x) && (nodeMin.y <= entMax.y && nodeMax.y >= entMin.y) && (nodeMin.z <= entMax.z && nodeMax.z >= entMin.z)){
        //colisiona
        if((nodeMin.x <= entMin.x && nodeMax.x >= entMax.x) && (nodeMin.y <= entMin.y && nodeMax.y >= entMax.y) && (nodeMin.z <= entMin.z && nodeMax.z >= entMax.z))
            return TypeCollision::Inside;
        return TypeCollision::Border;
    }else{
        return TypeCollision::No_Collision;
    }

}



// Comprueba si es visible el cuadrado en el frustum y pone los elementos que los contienen como dibujable en el motor
void Octree::UpdateVisibleObjects(RenderFacade* renderEngine){
<<<<<<< HEAD
    visible = renderEngine->FacadeOctreeInCamera(size*2.0, centralPosition);
    if(entitiesContained.size()>0){
=======
    if(entitiesContained.size()>0){
        visible = renderEngine->FacadeOctreeInCamera(size*2.0, centralPosition);
>>>>>>> ClippingOctree
        for(const auto& entity : entitiesContained){
            auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
            renderEngine->FacadeSetOctreeVisibleById(cId->id, visible);
        }
    } 

    for(int i = 0; i<numChilds; i++){
        childs[i]->UpdateVisibleObjects(renderEngine);
    }
}



// Para depuracion grafica dibujar el octree
void Octree::Draw(RenderFacade* renderEngine) const{
    for(int i = 0; i<numChilds; i++){
        childs[i]->Draw(renderEngine);
    }

    //if(entitiesContained.size()>0)
    //    DrawCube(renderEngine, 255, 255, 0);
    if(visible)
        DrawCube(renderEngine, 255, 0, 255);
    else
        DrawCube(renderEngine, 255, 0, 0);

}

void Octree::DrawCube(RenderFacade* renderEngine, int r, int g, int b) const{
        //cout << "( " << centralPosition.x << " , " << centralPosition.y << " , " << centralPosition.z << " ) --> " << size << "\n";
        glm::vec3 min0 = glm::vec3(centralPosition.x - size, centralPosition.y - size, centralPosition.z - size);
        glm::vec3 max0 = glm::vec3(centralPosition.x + size, centralPosition.y - size, centralPosition.z - size);
        renderEngine->Draw3DLine(min0, max0, r, g, b);

        glm::vec3 min1 = glm::vec3(centralPosition.x - size, centralPosition.y - size, centralPosition.z - size);
        glm::vec3 max1 = glm::vec3(centralPosition.x - size, centralPosition.y + size, centralPosition.z - size);
        renderEngine->Draw3DLine(min1, max1, r, g, b);

        glm::vec3 min2 = glm::vec3(centralPosition.x - size, centralPosition.y - size, centralPosition.z - size);
        glm::vec3 max2 = glm::vec3(centralPosition.x - size, centralPosition.y - size, centralPosition.z + size);
        renderEngine->Draw3DLine(min2, max2, r, g, b);

        glm::vec3 min3 = glm::vec3(centralPosition.x + size, centralPosition.y + size, centralPosition.z + size);
        glm::vec3 max3 = glm::vec3(centralPosition.x + size, centralPosition.y + size, centralPosition.z - size);
        renderEngine->Draw3DLine(min3, max3, r, g, b);

        glm::vec3 min4 = glm::vec3(centralPosition.x + size, centralPosition.y + size, centralPosition.z + size);
        glm::vec3 max4 = glm::vec3(centralPosition.x + size, centralPosition.y - size, centralPosition.z + size);
        renderEngine->Draw3DLine(min4, max4, r, g, b);

        glm::vec3 min5 = glm::vec3(centralPosition.x + size, centralPosition.y + size, centralPosition.z + size);
        glm::vec3 max5 = glm::vec3(centralPosition.x - size, centralPosition.y + size, centralPosition.z + size);
        renderEngine->Draw3DLine(min5, max5, r, g, b);


        glm::vec3 min6 = glm::vec3(centralPosition.x + size, centralPosition.y - size, centralPosition.z - size);
        glm::vec3 max6 = glm::vec3(centralPosition.x + size, centralPosition.y - size, centralPosition.z + size);
        renderEngine->Draw3DLine(min6, max6, r, g, b);

        glm::vec3 min7 = glm::vec3(centralPosition.x + size, centralPosition.y - size, centralPosition.z - size);
        glm::vec3 max7 = glm::vec3(centralPosition.x + size, centralPosition.y + size, centralPosition.z - size);
        renderEngine->Draw3DLine(min7, max7, r, g, b);


        glm::vec3 min8 = glm::vec3(centralPosition.x - size, centralPosition.y + size, centralPosition.z - size);
        glm::vec3 max8 = glm::vec3(centralPosition.x + size, centralPosition.y + size, centralPosition.z - size);
        renderEngine->Draw3DLine(min8, max8, r, g, b);

        glm::vec3 min9 = glm::vec3(centralPosition.x - size, centralPosition.y + size, centralPosition.z - size);
        glm::vec3 max9 = glm::vec3(centralPosition.x - size, centralPosition.y + size, centralPosition.z + size);
        renderEngine->Draw3DLine(min9, max9, r, g, b);


        glm::vec3 min10 = glm::vec3(centralPosition.x - size, centralPosition.y - size, centralPosition.z + size);
        glm::vec3 max10 = glm::vec3(centralPosition.x - size, centralPosition.y + size, centralPosition.z + size);
        renderEngine->Draw3DLine(min10, max10, r, g, b);

        glm::vec3 min11 = glm::vec3(centralPosition.x - size, centralPosition.y - size, centralPosition.z + size);
        glm::vec3 max11 = glm::vec3(centralPosition.x + size, centralPosition.y - size, centralPosition.z + size);
        renderEngine->Draw3DLine(min11, max11, r, g, b);        
}
