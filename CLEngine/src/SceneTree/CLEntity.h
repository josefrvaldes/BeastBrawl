#pragma once

#include <stdint.h>
namespace CLE{

class CLEntity{

    CLEntity(unsigned int idEntity) : id(idEntity){}
    ~CLEntity();

    private:

        unsigned int id;
};

}
