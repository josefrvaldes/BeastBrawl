#include <Systems/Utils.h>
#include <Components/CBufferOnline.h>
#include <iostream>
#include "Game.h"
#include "Constants.h"
#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char *argv[]) {

    // CBufferOnline buff;
    // int64_t timeSent = Utils::getMillisSinceEpoch(); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewCalculated(vec3(0,0,0), vec3(0,0,0)); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewCalculated(vec3(0,0,0), vec3(0,0,0)); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewCalculated(vec3(0,0,0), vec3(0,0,0)); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewReceivedOnline(timeSent, vector<Constants::InputTypes>()); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // timeSent = Utils::getMillisSinceEpoch(); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewCalculated(vec3(0,0,0), vec3(0,0,0)); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewCalculated(vec3(0,0,0), vec3(0,0,0)); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewCalculated(vec3(0,0,0), vec3(0,0,0)); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewCalculated(vec3(0,0,0), vec3(0,0,0)); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewCalculated(vec3(0,0,0), vec3(0,0,0)); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewReceivedOnline(timeSent, vector<Constants::InputTypes>()); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // timeSent = Utils::getMillisSinceEpoch(); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewCalculated(vec3(0,0,0), vec3(0,0,0)); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewCalculated(vec3(0,0,0), vec3(0,0,0)); std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // buff.InsertNewReceivedOnline(timeSent, vector<Constants::InputTypes>()); std::this_thread::sleep_for(std::chrono::milliseconds(50));

    if(argc == 2) {
        Constants::SERVER_HOST = argv[1];
    }
    cout << "La ip del servidor será " << Constants::SERVER_HOST << endl;

    //try {
    Game* game = Game::GetInstance();

    game->InitGame();

    if(Constants::RENDER_ENGINE == Constants::RenderEngine::CLOVER){
        game->SetState(State::States::INTRO);
        
    }else if(Constants::RENDER_ENGINE == Constants::RenderEngine::IRRLICHT){
        game->SetState(State::States::MENU);
    }
    //game->SetState(State::States::INGAME_SINGLE);
    game->MainLoop();
    game->TerminateGame();
    //} catch (exception &e) {
    //    cout << "Algo ha fallado " << e.what() << endl;
    //}

    return 0;
}