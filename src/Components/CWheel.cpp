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
 
 
 #include "CWheel.h"

#include "CId.h"


CWheel::CWheel(string pathTopLeft,string pathTopRight,string pathBottomLeft,string pathBottomRight,glm::vec3 _offsetTopLeft,glm::vec3 _offsetTopRight,glm::vec3 _offsetBottomLeft,glm::vec3 _offsetBottomRight,glm::vec3 _rotationTopLeft,glm::vec3 _rotationTopRight,glm::vec3 _rotationBottomLeft,glm::vec3 _rotationBottomRight,glm::vec3 _scaleTopLeft,glm::vec3 _scaleTopRight,glm::vec3 _scaleBottomLeft,glm::vec3 _scaleBottomRight){
    m_compType = CompType::WheelComp;

    shared_ptr<CId> cIdTL  = make_shared<CId>();
    shared_ptr<CId> cIdTR   = make_shared<CId>();
    shared_ptr<CId> cIdBL   = make_shared<CId>();
    shared_ptr<CId> cIdBR   = make_shared<CId>();
    IdWheelTopLeft = cIdTL->id;
    IdWheelTopRight = cIdTR->id;
    IdWheelBottomLeft = cIdBL->id;
    IdWheelBottomRight = cIdBR->id;

    meshTopLeft = pathTopLeft;
    meshTopRight = pathTopRight;
    meshBottomLeft = pathBottomLeft;
    meshBottomRight = pathBottomRight;

    offsetTopLeft = _offsetTopLeft;
    offsetTopRight = _offsetTopRight;
    offsetBottomLeft = _offsetBottomLeft;
    offsetBottomRight = _offsetBottomRight;

    rotationTopLeft = _rotationTopLeft;
    rotationTopRight = _rotationTopRight;
    rotationBottomLeft = _rotationBottomLeft;
    rotationBottomRight = _rotationBottomRight;

    scaleTopLeft = _scaleTopLeft;
    scaleTopRight = _scaleTopRight;
    scaleBottomLeft = _scaleBottomLeft;
    scaleBottomRight = _scaleBottomRight;
}

