#include "CIDOnline.h"


/**
 * Este constructor sirve para crear un id nuevo de cero
 */
CIDOnline::CIDOnline(uint16_t idPlayer) : idOnline{static_cast<uint16_t>(std::stoi(to_string(idPlayer) + to_string(++id)))}, collided{false} {
    m_compType = CompType::CIDOnlineComp;
}

/**
 * Este constructor sirve para crear un id a partir de uno existente
 * Este bool no sirve para nada, es solo porque necesito dos constructores del mismo tipo
 * pero que hagan dos cosas distintas, así que le he metido ese bool para distinguirlos
 */
CIDOnline::CIDOnline(uint16_t idOnline_, bool noSirveParaNadaEsteBool) : idOnline{idOnline_} {
    m_compType = CompType::CIDOnlineComp;
}