#pragma once

#include <chrono>
#include <glm/vec3.hpp>
#include <iostream>
#include <list>
#include <string>

#include "../Constants.h"
#include "../Systems/Utils.h"
#include "Component.h"

using namespace std;

struct BuffElement {
    
   private:
    BuffElement() = delete;
    // cada vez que recibamos un paquete online de un jugador, se llamará a este constructor.
    // como veis, recibiremos la hora a la que se envió y los inputs. La pos y rot se calcularán con los datos anteriores en el Physics.cpp
    BuffElement(int64_t timeSent_, vector<Constants::InputTypes> inputs_)
        : time{Utils::getMillisSinceEpoch()}, timeSent{timeSent_}, receivedForReal{true}, inputs{inputs_}, pos{}, rot{} {};

    // cada vez que se ejecuta un update y NO hemos recibido un paquete de un cliente, llamaremos a este constructor.
    // solo recibe la pos actual y la rotación actual. Nos servirá para hacer correcciones cuando llegue un paquete de verdad.
    BuffElement(glm::vec3 pos_, glm::vec3 rot_)
        : time{Utils::getMillisSinceEpoch()}, timeSent{0}, receivedForReal{false}, inputs{}, pos{pos_}, rot{rot_} {};

   public:
    int64_t time;
    int64_t timeSent;
    bool receivedForReal{false};
    vector<Constants::InputTypes> inputs;
    glm::vec3 pos;
    glm::vec3 rot;

    friend class CBufferOnline;
};

class CBufferOnline : public Component {
   public:
    CBufferOnline();
    ~CBufferOnline();
    void InsertNewReceivedOnline(int64_t time, vector<Constants::InputTypes> inputs);
    void InsertNewCalculated(glm::vec3 pos_, glm::vec3 rot_);

    bool newInputReceived{false};
    list<BuffElement> elems;
};