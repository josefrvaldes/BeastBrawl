#pragma once

#include "Component.h"
#include <string>
#include <iostream>

using namespace std;

class CId : public Component
{
public:
    CId();
    ~CId();

    friend ostream &operator<<(ostream &out, CId &c)
    {
        out << "(" << c.id << ")";
        return out;
    }

    uint16_t id;

private:
    static uint16_t numIds;  //Lleva la cuenta del id
};
