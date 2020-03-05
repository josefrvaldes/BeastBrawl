#include "NamePlate.h"
#include <Components/CId.h>
#include <Components/CNamePlate.h>
#include <Components/CType.h>
#include <Components/CShader.h>

class Position;
using namespace std;


NamePlate::NamePlate(int _id)
{

    string vertexShader = "CLEngine/src/Shaders/vertex.glsl";
    string fragmentShader = "CLEngine/src/Shaders/fragment.glsl";

    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cType   = make_shared<CType>(ModelType::Text);
    shared_ptr<Component> cNamePlate = make_shared<CNamePlate>(_id);
    shared_ptr<CShader> cShader = make_shared<CShader>(vertexShader,fragmentShader);

    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cNamePlate);
    AddComponent(cShader);
}

NamePlate::~NamePlate()
{
    
}