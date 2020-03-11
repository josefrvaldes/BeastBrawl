#include "ManGameRules.h"
#include "../Entities/Clock.h"
#include <iostream>

ManGameRules::ManGameRules() {
    // creamos el reloj global


}
void ManGameRules::CreateGlobalClock() {
    globalClock = make_unique<Clock>();
}


ManGameRules::~ManGameRules() {
    std::cout << "Llamando al destructor de ManGameRules" << std::endl;
}

void ManGameRules::SubscribeToEvents() {

}