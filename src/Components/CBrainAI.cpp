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
        //cout  << "111111111111111111111111111111111111111 el tiempo del totem es de: " << timeFollowDirect << endl;

    }else if(dif == BrainAIDifficult::NORMAL){
        totalTried =2;
        numTried =1; // empezamos ya en el ultimo intento porque la primera vez no queremos pensar (al inicio de la partida)
        // Tiempo para ponerse seria, e ir a por el que tiene el totem
        timeFollowDirect = timeTotem * 0.5;
        //cout  << "111111111111111111111111111111111111111 el tiempo del totem es de: " << timeFollowDirect << endl;
    }else if(dif == BrainAIDifficult::EASY){
        totalTried =3;
        numTried =2; // empezamos ya en el ultimo intento porque la primera vez no queremos pensar (al inicio de la partida)
        // Tiempo para ponerse seria, e ir a por el que tiene el totem
        timeFollowDirect = timeTotem * 0.3;
        //cout  << "111111111111111111111111111111111111111 el tiempo del totem es de: " << timeFollowDirect << endl;
    }//else{
        //cout << "BROOOO, que dificultad quieres? te la has inventado " << endl;
    //}
    difficult = dif;
}
 


void CBrainAI::CleanVisionRange(){
    carInVision.clear();
    boxInVision.clear();
    totemInVision = nullptr;
}