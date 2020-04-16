#include "ManAI.h"
#include <chrono>


void ManAI::addBehavior(CarAI* carAI_, SystemAI* sysAI_, double frec, double pharse){


    //Create AI Behavior Event
    BehaviorRecord newBehaviour;

    newBehaviour.b_carAI        = carAI_;
    newBehaviour.b_sysAI       = sysAI_;
    newBehaviour.b_frecuency    = frec;
    newBehaviour.b_pharse       = pharse;

    behaviours.push_back(newBehaviour);

}

void ManAI::Update(){
//def run(timeToRun):
    std::chrono::duration<double> timeToRun_sec;

    double maxTimeBehaviour = 100;

    double totalTime = 0;

    // Increment the frame number
    frameActual += 1;
    // Keep a list of behaviors to run --> runThese

    // Go through each behavior
    for(auto currentBehaviour : behaviours){
        if( currentBehaviour.b_frecuency / (frameActual+currentBehaviour.b_pharse) )
            runThese.push(currentBehaviour);
    }

    // Keep track of the current time
    auto lastTime = std::chrono::system_clock::now();
    // Find the number of behaviors we need to run
    auto numToRun = runThese.size();


    for(int i=0; i<numToRun; i++){
        auto currentTime = std::chrono::system_clock::now();
        timeToRun_sec -= currentTime - lastTime;

        auto availabeTime = timeToRun_sec / ( numToRun - i);

        if( availabeTime.count() > maxTimeBehaviour){
            //run
        }
        
        lastTime = currentTime;
    }

}