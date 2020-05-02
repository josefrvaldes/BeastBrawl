#include "CBrainAI.h"

CBrainAI::CBrainAI(){
    m_compType = CompType::BrainAIComp;
}
 


void CBrainAI::CleanVisionRange(){
    carInVision.clear();
    boxInVision.clear();
    totemInVision = nullptr;
}