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
 
 
 #include "CCamera.h"

CCamera::CCamera(float _tarx, float _tary, float _tarz, float _rotExtraY)
: tarX(_tarx), tarY(_tary), tarZ(_tarz), rotExtraY(_rotExtraY)
{
    m_compType = CompType::CameraComp;
    this->ApplyPhysics(0.0,0.0,0.0, 0.0);
}  
 

// vamos a aplicar fisicas a la camara, lo que implica que tendra su propio movimiento, siempre dependiente de alguna entidad
// pero con sus propios parametros de velocidad, aceleracion y frenado.
void CCamera::ApplyPhysics(float maxSpeed_, float acc_, float revMaxSpeed_, float slowDown_){

    maxSpeed        = maxSpeed_;        // velocidad maxima que la entidad
    acceleration    = acc_*0.9;         // aceleracion un poco mas lenta que la de la entidad
    reverseMaxSpeed = revMaxSpeed_;     // marcha atras igual que la entidad
    slowDown        = slowDown_;
}