#ifndef __FACADE_H__
#define __FACADE_H__

#include <iostream>
#include <cstdint>

class Facade {
    public:
        Facade();
        virtual ~Facade() = default;
        virtual void FacadeInit() = 0;
        virtual void FacadeCreateSphere();

    private:
};


#endif 