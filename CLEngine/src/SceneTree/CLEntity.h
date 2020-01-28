#pragma once

#include <stdint.h>
namespace CLE{

class CLEntity{
    public:
        CLEntity(unsigned int idEntity) : id(idEntity){}
        ~CLEntity(){};

        unsigned int GetID() { return id; }

    private:

        unsigned int id;
};

}
