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
 
 
 #include "CWayPoint.h"

CWayPoint::CWayPoint(){
    m_compType = CompType::WayPointComp;
    position.x = 0;
    position.y = 0;
    position.z = 0;
    radious = 30.0f;
}

CWayPoint::CWayPoint(glm::vec3 _pos, int _type, int _id)
: position(_pos), type(_type), id(_id){
    m_compType = CompType::WayPointComp;
    if(type == 1) // si el tipo es de PowerUp, nos aseguramos que pasamos por el
        radious = 10.0f;
    else
        radious = 20.0f;
    
}

CWayPoint::~CWayPoint(){
    
}