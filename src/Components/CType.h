#pragma once

#include "Component.h"

#include <string>
#include <iostream>

using namespace std;

enum ModelType{
    Sphere,
    Cube,
    StaticMesh,
    AnimatedMesh,
    Text
};

class CType : public Component{

public:
    CType(ModelType model);
    ~CType(){};

    friend ostream &operator<<(ostream &out, CType &c)
    {
        out << "(" << c.type << ")";
        return out;
    }

    ModelType type;
};
