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
 
 
 #include "CPowerUp.h"

CPowerUp::CPowerUp(): typePowerUp(typeCPowerUp::None){
    m_compType = CompType::PowerUpComp;
}

CPowerUp::CPowerUp(typeCPowerUp _typePowerUp): typePowerUp(_typePowerUp){
    m_compType = CompType::PowerUpComp;
}
/*
CPowerUp::CPowerUp(glm::vec3 _pos, int type, int id)
: position(_pos), m_type(type), m_id(id){
    m_compType = CompType::WayPointComp;
    radious = 30.0f;
    padre = nullptr;
    isFrontera = false;
    isInterior = false;
}*/

CPowerUp::~CPowerUp(){
}