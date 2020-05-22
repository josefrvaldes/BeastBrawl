/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
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
    BuffElement(int64_t time_, int64_t timeSent_, vector<Constants::InputTypes> inputs_, float speed_, float wheelRotation_, float skidDeg_, float skidRotation_)
        : time{time_}, timeSent{timeSent_}, receivedForReal{true}, inputs{inputs_}, pos{}, rot{}, speed{speed_}, wheelRotation{wheelRotation_}, skidDeg{skidDeg_}, skidRotation{skidRotation_} {};

    // cada vez que se ejecuta un update y NO hemos recibido un paquete de un cliente, llamaremos a este constructor.
    // solo recibe la pos actual y la rotación actual. Nos servirá para hacer correcciones cuando llegue un paquete de verdad.
    BuffElement(int64_t time_, glm::vec3 pos_, glm::vec3 rot_, float speed_, float wheelRotation_, float skidDeg_, float skidRotation_)
        : time{time_}, timeSent{0}, receivedForReal{false}, inputs{}, pos{pos_}, rot{rot_}, speed{speed_}, wheelRotation{wheelRotation_}, skidDeg{skidDeg_}, skidRotation{skidRotation_} {};

   public:
    int64_t time;
    int64_t timeSent;
    bool receivedForReal{false};
    vector<Constants::InputTypes> inputs;
    glm::vec3 pos;
    glm::vec3 rot;
    float speed;
    float wheelRotation;
    float skidDeg;
    float skidRotation;

    friend class CBufferOnline;
};

class CBufferOnline : public Component {
   public:
    CBufferOnline();
    ~CBufferOnline();
    void InsertNewReceivedOnline(int64_t time_, int64_t timeSent_, vector<Constants::InputTypes> inputs, float speed, float wheelRotation, float skidDeg, float skidRotation);
    void InsertNewCalculated(int64_t time_, glm::vec3 pos_, glm::vec3 rot_, float speed, float wheelRotation, float skidDeg, float skidRotation);

    friend ostream &operator<<(ostream &out, CBufferOnline &c) {
        const std::list<BuffElement>::iterator it;
        out << "JUG2Mostramos el estado del CBufferOnline" << endl;
        for (std::list<BuffElement>::iterator it = c.elems.begin(); it != c.elems.end(); ++it) {
            if (it->receivedForReal) {
                out << "JUG2\treceived_for_real timeSent[" << Utils::getISOCurrentTimestampMillis(it->timeSent) << "] time[" << Utils::getISOCurrentTimestampMillis(it->time) << "] y speed[" << it->speed << "]" << endl;
            } else {
                out << "JUG2\t     not_for_real     time[" << Utils::getISOCurrentTimestampMillis(it->time) << "] pos[" << it->pos.x << "," << it->pos.y << "," << it->pos.z << "] rot[" << it->rot.x << "," << it->rot.y << "," << it->rot.z << "] y speed[" << it->speed << "]" << endl;
            }
        }

        return out << endl
                   << endl;
    }

    bool newInputReceived{false};
    list<BuffElement> elems;
};