#include "Manager.h"

#include <iostream>

Manager::Manager() {
}
Manager::~Manager() {
    cout << "Se llama al destructor de manager" << endl;
    entities.clear();
    entities.shrink_to_fit();
}