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
 * @author Rubén Rubio Martínez <https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/>
 * 
 */
 
 
 #pragma once

#include <stdint.h>

namespace CLE{
//! Clase para crear colores
//! Usamos esta clase para enviarle objetos a algunos metodos que reciben colores
class CLColor{
    public:
        //! Constructor por defecto
        CLColor();
        //! Constructor con parámetros
        //! @param red Valor del rojo
        //! @param green Valor del verde
        //! @param blue Valor del azul
        //! @param alpha Valor de la trasparencia
        //! @note Valores siempre entre 0 y 255
        CLColor(uint8_t red,uint8_t green,uint8_t blue,uint8_t alpha);
        ~CLColor(){};

        //! Devuelve el valor del rojo
        //! @returns red
        uint8_t GetRed()   const { return r;};
        //! Devuelve el valor del verde
        //! @returns green
        uint8_t GetGreen() const { return g;};
        //! Devuelve el valor del azul
        //! @returns blue
        uint8_t GetBlue()  const { return b;};
        //! Devuelve el valor de la trasparencia
        //! @returns alpha
        uint8_t GetAlpha() const { return a;};

        //! Devuelve el valor del rojo normalizado
        float GetRedNormalized()   const { return r/255.0;};
        //! Devuelve el valor del verde normalizado
        float GetGreenNormalized() const { return g/255.0;};
        //! Devuelve el valor del azul normalizado
        float GetBlueNormalized()  const { return b/255.0;};
        //! Devuelve el valor de la trasparencia normalizado
        float GetAlphaNormalized() const { return a/255.0;};
    private:
        uint8_t r; 
        uint8_t g;
        uint8_t b;
        uint8_t a;
};
};
