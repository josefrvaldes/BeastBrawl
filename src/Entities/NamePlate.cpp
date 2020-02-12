#include "NamePlate.h"
#include <Components/CId.h>
#include <Components/CNamePlate.h>
#include <Components/CType.h>

class Position;
using namespace std;


NamePlate::NamePlate(int _id)
{
    shared_ptr<Component> cId   = make_shared<CId>();
    shared_ptr<Component> cType   = make_shared<CType>(ModelType::Text);
    shared_ptr<Component> cNamePlate = make_shared<CNamePlate>(_id);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cNamePlate);
}

NamePlate::~NamePlate()
{
    
}