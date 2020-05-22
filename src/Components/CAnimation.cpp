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
 
 
 #include "CAnimation.h"
#include <memory>

CAnimation::CAnimation()
{
    m_compType = CompType::AnimationComp;
}

/*CAnimation::CAnimation(Animation animClose_)
: activeAnimation{std::make_shared<Animation>(animClose_)} ,animations{std::make_shared<Animation>(animClose_)}
{
    m_compType = CompType::AnimationComp;
}

CAnimation::CAnimation(Animation animClose_, Animation animMedium_, Animation animFar_)
: activeAnimation{std::make_shared<Animation>(animClose_)}, animations{std::make_shared<Animation>(animClose_), std::make_shared<Animation>(animMedium_), std::make_shared<Animation>(animFar_)}
{
    m_compType = CompType::AnimationComp;
}
*/
