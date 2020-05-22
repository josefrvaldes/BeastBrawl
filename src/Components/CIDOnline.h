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

#include "Component.h"

#include <iostream>
#include <string>
#include <memory>

using namespace std;

class CIDOnline : public Component {
   public:
    CIDOnline() = delete; 
    CIDOnline(uint16_t idOnline, bool noSirveParaNadaEsteBool); // para crear a partir de un id existente
    CIDOnline(uint16_t idPlayer); // para crear uno nuevo de cero
    ~CIDOnline() = default;

    /*shared_ptr<CIDOnline> CreateNewCID(uint16_t idPlayer) {
        struct make_shared_enabler : public CIDOnline {};
        return std::make_shared<make_shared_enabler>(idPlayer);
    }

    shared_ptr<CIDOnline> CreateCIDFromExistingId(uint16_t id) {
        struct make_shared_enabler : public CIDOnline {};
        return make_shared<make_shared_enabler>(id, true);
    }*/

    friend ostream &operator<<(ostream &out, CIDOnline &c) {
        out << "(" << c.id << ")";
        return out;
    }

    void ResetNumIds() { id = 0; }

    uint16_t idOnline{0};
    bool collided;

   private:
    inline static uint16_t id{0};  //Lleva la cuenta del id
};
