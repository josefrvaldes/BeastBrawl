#include "ManAI.h"
#include <chrono>


using namespace std::chrono;


ManAI::ManAI(){

}

void ManAI::addBehavior(CarAI* carAI_, SystemAI* sysAI_, int frec, int pharse, double timeMax){


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
    double timeToRun_sec = 0.0035 * 10000 * 100000; // en nanosegundos = 0.0035 segundos .. de los que nos sobran 0.34 :)

    // Increment the frame number
    frameActual += 1;
    // Keep a list of behaviors to run --> runThese

    //cout << "antes de entrar tenemos:  " << runThese.size() << endl;
    // Go through each behavior
    for(auto currentBehaviour : behaviours){
        if((frameActual+currentBehaviour.b_pharse) % currentBehaviour.b_frecuency == 0) 
            runThese.push(currentBehaviour);
    }

    //cout << "una vez llenado tenemos:  " << runThese.size() << endl;
    // Keep track of the current time
    auto lastTime = system_clock::now();
    // Find the number of behaviors we need to run
    auto numToRun = runThese.size();
    //cout << "antes de entrar el tiempo que tengo es:   " << timeToRun_sec << endl;
    for(long unsigned int i=0; i<numToRun; i++){
        auto currentTime = system_clock::now();
        //cout << "le restamos   :   " << duration_cast<nanoseconds>(currentTime - lastTime).count() << endl;
        timeToRun_sec -= duration_cast<nanoseconds>(currentTime - lastTime).count();
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
    //cout << "cuando salimos tenemos :  " << runThese.size() << " con un timpo sobrante de: " << timeToRun_sec <<endl;

}