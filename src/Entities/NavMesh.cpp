#include "NavMesh.h"
#include <Components/CId.h>
#include <Components/CType.h>
#include <Components/CTexture.h>
#include <Components/CMesh.h>
#include <Components/CTransformable.h>
#include <Components/CNavMesh.h>
#include <Components/CDimensions.h>

#include <iostream>

class Position;
using namespace std;


NavMesh::NavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, const vector<int>& waypoints)
{
    string texture = "spheremap.jpg";
    string mesh    = "media/ninja.b3d";

    //Dividimos entre 10 porque con scale 1 irrlicht cuenta como 10, de esta manera si le decimos que queremos 10 de altura lo tomara como 1 y en irrlicht sera 10
    glm::vec3 scale = glm::vec3(width/10.0,height/10.0,depth/10.0);
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::Cube);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CNavMesh> cNavMesh   = make_shared<CNavMesh>(waypoints);
    shared_ptr<CDimensions> cDimensions   = make_shared<CDimensions>(width,height,depth);

    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cNavMesh);
    AddComponent(cDimensions);


}




NavMesh::~NavMesh()
{
    
}