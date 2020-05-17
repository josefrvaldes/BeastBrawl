#include "Shield.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "../Components/CParentNode.h"
#include <Components/CShader.h>
#include <iostream>
#include "../Constants.h"

using namespace std;

Shield::Shield(){}


Shield::Shield(uint16_t parentId, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
    shared_ptr<CParentNode> cParentNode = make_shared<CParentNode>(parentId);
    if(parentId != -1){
        AddComponent(cParentNode);

    }
    shared_ptr<CMesh> cMesh = make_shared<CMesh>("shield.obj");
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(position,rotation,scale);
    shared_ptr<CType> cType   = make_shared<CType>(ModelType::Shield);
    shared_ptr<CShader> cShader = make_shared<CShader>("CLEngine/src/Shaders/basicShader.vert","CLEngine/src/Shaders/basicShader.frag");

    AddComponent(cShader);
    AddComponent(cId);
    AddComponent(cTransformable);
    AddComponent(cType);
    AddComponent(cParentNode);
    AddComponent(cMesh);
}


Shield::~Shield()
{

}
