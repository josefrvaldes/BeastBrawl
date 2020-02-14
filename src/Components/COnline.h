#pragma once


#include "Component.h"
#include <vector>
#include "../Constants.h"
#include <time.h>
#include <chrono>

using namespace std;

class COnline : public Component{
public:
    COnline();
    ~COnline();

    uint16_t idClient = -1;
    vector<Constants::InputTypes> inputs;
    time_t lastInputUpdate;
    time_t lastWorldUpdate;
};