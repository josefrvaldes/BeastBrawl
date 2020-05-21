#include "CBrainAI.h"

CBrainAI::CBrainAI(){
    m_compType = CompType::BrainAIComp;

    //fuzzy logic del coche para el movimiento (giro y aceleracion)
    fuzzyLogic = make_shared<SystemFuzzyLogicAI>();
}

CBrainAI::CBrainAI(BrainAIDifficult dif, float timeTotem) : CBrainAI() {

    if(dif == BrainAIDifficult::DIFFICULT){
        totalTried =1;
        numTried =0; // empezamos ya en el ultimo intento porque la primera vez no queremos pensar (al inicio de la partida)
        // Tiempo para ponerse seria, e ir a por el que tiene el totem
        timeFollowDirect = timeTotem * 0.8;

    }else if(dif == BrainAIDifficult::NORMAL){
        totalTried =2;
        numTried =1; // empezamos ya en el ultimo intento porque la primera vez no queremos pensar (al inicio de la partida)
        // Tiempo para ponerse seria, e ir a por el que tiene el totem
        timeFollowDirect = timeTotem * 0.5;
    }else if(dif == BrainAIDifficult::EASY){
        totalTried =3;
        numTried =2; // empezamos ya en el ultimo intento porque la primera vez no queremos pensar (al inicio de la partida)
        // Tiempo para ponerse seria, e ir a por el que tiene el totem
        timeFollowDirect = timeTotem * 0.3;
    }else{
        cout << "BROOOO, que dificultad quieres? te la has inventado " << endl;
    }
    difficult = dif;
}
 


void CBrainAI::CleanVisionRange(){
    carInVision.clear();
    boxInVision.clear();
    totemInVision = nullptr;
}