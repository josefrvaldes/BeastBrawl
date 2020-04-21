#include "CAnimation.h"


CAnimation::CAnimation(Animation animClose_)
: animations{animClose_}, activeAnimation{animClose_}
{
    m_compType = CompType::AnimationComp;
}

CAnimation::CAnimation(Animation animClose_, Animation animMedium_, Animation animFar_)
: animations{animClose_, animMedium_, animFar_}, activeAnimation{animClose_}
{
    m_compType = CompType::AnimationComp;
}

