#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <stdlib.h>
#include "../../lib/glm/vec3.hpp"
#include "Manager.h"
#include "ManCar.h"

using namespace std;

class ManNamePlate : public Manager{
public:
    ManNamePlate(ManCar* cars); 
    ~ManNamePlate();




private:

};