#pragma once

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;

enum ModelType{
    Sphere,
    Cube,
    StaticMesh,
    AnimatedMesh
};

class CType : public Component{

public:
    CType(ModelType model);
    ~CType();

    ModelType GetType() { return m_type; }

    friend ostream &operator<<(ostream &out, CType &c)
    {
        out << "(" << c.GetType()<< ")";
        return out;
    }

private:
    ModelType m_type;
};
