#include "CAnimation.h"


// esto HAY que quitarlo, porque estamos guardando el triple de información de manera innecesaria.
// tenemos que asegurarnos de que todas las entidades con malla tengan 3
CAnimation::CAnimation(std::string pathCerca, uint8_t numKeyFrames_)
: mesh{pathCerca}, numKeyFrames{numKeyFrames_}
{
    m_compType = CompType::AnimationComp;
}

