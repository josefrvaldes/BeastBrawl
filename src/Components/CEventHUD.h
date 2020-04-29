#pragma once

#include "Component.h"

#include <string>
#include <chrono>

#include <iostream>

enum class eventHUDType { NONE, STOLE, CATCH, LOSE };

class CEventHUD : public Component {
    public:
        CEventHUD();
        ~CEventHUD() = default;

        eventHUDType typeEvent { eventHUDType::NONE };
        uint16_t characterEventHUD { 0 };
        std::chrono::time_point<std::chrono::system_clock> timeStart;        // reloj que contabiliza el tiempo que esta activo un eventhud
        const uint32_t DURATION_TIME {3};
        int64_t accumulatedTime { 0 };
};