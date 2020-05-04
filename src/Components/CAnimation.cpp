#include "CAnimation.h"
#include <memory>


CAnimation::CAnimation(Animation animClose_)
: activeAnimation{std::make_shared<Animation>(animClose_)} ,animations{std::make_shared<Animation>(animClose_)}
{
    m_compType = CompType::AnimationComp;
}

CAnimation::CAnimation(Animation animClose_, Animation animMedium_, Animation animFar_)
: activeAnimation{std::make_shared<Animation>(animClose_)}, animations{std::make_shared<Animation>(animClose_), std::make_shared<Animation>(animMedium_), std::make_shared<Animation>(animFar_)}
{
    m_compType = CompType::AnimationComp;
}

