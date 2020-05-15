#pragma once


#include "Component.h"
#include <vector>
#include "../Constants.h"
#include <time.h>
#include <chrono>
#include <unordered_map>

using namespace std;

class COnline : public Component{
public:
    COnline();
    ~COnline();

    uint16_t idClient = -1;
    vector<Constants::InputTypes> inputs;

    // estos 3 valores son para la sincronización del reloj
    unordered_map<uint16_t, int64_t> timeSyncClock;
    unordered_map<uint16_t, float> currentTurnout;
    unordered_map<uint16_t, int8_t> numMeasurements;
};