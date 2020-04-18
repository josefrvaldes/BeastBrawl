#include "ManAI.h"
#include <chrono>


using namespace std::chrono;


ManAI::ManAI(){

}

void ManAI::addBehavior(CarAI* carAI_, SystemAI* sysAI_, float frec, float pharse, double timeMax){


    //Create AI Behavior Event
    BehaviorRecord newBehaviour;

    newBehaviour.b_carAI        = carAI_;
    newBehaviour.b_sysAI        = sysAI_;
    newBehaviour.b_frecuency    = frec;
    newBehaviour.b_pharse       = pharse;
    newBehaviour.b_timeMax      = timeMax;

    behaviours.push_back(newBehaviour);

}

void ManAI::Update(){
//def run(timeToRun):
    double timeToRun_sec = 0.00015*10;;

    // Increment the frame number
    frameActual += 1;
    // Keep a list of behaviors to run --> runThese

    cout << "antes de entrar tenemos:  " << runThese.size() << endl;
    // prueba con todos a ver que tal.. parte crucial de toda la puta semana
    for(auto currentBehaviour : behaviours){
        runThese.push(currentBehaviour);
    }
    cout << "una vez llenado tenemos:  " << runThese.size() << endl;
    auto numToRun = runThese.size();
    for(long unsigned int i=0; i<numToRun; i++){
        runThese.front().b_sysAI->update(runThese.front().b_carAI);
        runThese.pop();
    }
    cout << "cuando salimos tenemos :  " << runThese.size() << endl;
    /*
    // Go through each behavior
    for(auto currentBehaviour : behaviours){
        if( currentBehaviour.b_frecuency / (frameActual+currentBehaviour.b_pharse) )
            runThese.push(currentBehaviour);
    }
    // Keep track of the current time
    auto lastTime = system_clock::now();
    // Find the number of behaviors we need to run
    auto numToRun = runThese.size();
    for(long unsigned int i=0; i<numToRun; i++){
        auto currentTime = system_clock::now();
        timeToRun_sec -= duration_cast<milliseconds>(currentTime - lastTime).count();
        //auto availabeTime = timeToRun_sec / ( numToRun - i);
        if( timeToRun_sec > runThese.front().b_timeMax){
            //run
            runThese.front().b_sysAI->update(runThese.front().b_carAI);
            runThese.pop();
        }else{
            return;
        }
        lastTime = currentTime;
    }
    */



}