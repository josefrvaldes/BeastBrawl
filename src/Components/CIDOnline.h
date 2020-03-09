#pragma once

#include "Component.h"

#include <iostream>
#include <string>

using namespace std;

class CIDOnline : public Component {
   public:
    CIDOnline() = delete;
    CIDOnline(uint16_t idPlayer) ;
    ~CIDOnline() = default;

    friend ostream &operator<<(ostream &out, CIDOnline &c) {
        out << "(" << c.id << ")";
        return out;
    }

    void ResetNumIds() { id = 0; }

    uint16_t idOnline{0};

   private:
    inline static uint16_t id{0};  //Lleva la cuenta del id
};
