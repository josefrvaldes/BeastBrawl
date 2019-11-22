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

    uint16_t GetId() { return m_id; }

    friend ostream &operator<<(ostream &out, CId &c)
    {
        out << "(" << c.GetId()<< ")";
        return out;
    }

private:
    uint16_t m_id;
    static uint16_t numIds;  //Lleva la cuenta del id
};
