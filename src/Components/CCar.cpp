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
 
 
 #include "CCar.h"

CCar::CCar(mainCharacter _pj, float _weight, float _maxSpeed, float _acceleration)
: character(_pj), weight(_weight), maxSpeed(_maxSpeed), acceleration(_acceleration), skidState(SkidState::DISABLED)
{
    m_compType = CompType::CarComp;
    skidStart = system_clock::now();
}
 


// To-Do: temporal hasta que se ajuste el derrape
CCar::CCar(mainCharacter _pj, float _weight, float _maxSpeed, float _acceleration, float maxWheelRot)
: character(_pj), weight(_weight), maxSpeed(_maxSpeed), acceleration(_acceleration), maxWheelRotation(maxWheelRot), skidState(SkidState::DISABLED)
{
    m_compType = CompType::CarComp;
    skidStart = system_clock::now();
}

