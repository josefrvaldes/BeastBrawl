#pragma once

#include "Component.h"

#include "../Constants.h"
#include <chrono>
#include <glm/vec3.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "../Systems/Utils.h"

using namespace std;

struct BuffElement {
    BuffElement() = delete;
    BuffElement(vector<Constants::InputTypes> inputs_, glm::vec3 pos_, glm::vec3 rot_)
        : time{Utils::getMillisSinceEpoch()}, inputs{inputs_}, pos{pos_}, rot{rot_} {};

    int64_t time;
    vector<Constants::InputTypes> inputs;
    glm::vec3 pos;
    glm::vec3 rot;
};

class CBufferOnline : public Component {
   public:
    CBufferOnline();
    ~CBufferOnline();

    vector<BuffElement > elems;
};