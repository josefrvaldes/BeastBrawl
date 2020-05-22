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
CIDOnline::CIDOnline(uint16_t idOnline_, bool noSirveParaNadaEsteBool) : idOnline{idOnline_}, collided{false} {
    m_compType = CompType::CIDOnlineComp;
}