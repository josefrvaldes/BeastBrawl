#include "Manager.h"

#include <iostream>

Manager::Manager() {
}
Manager::~Manager() {
    entities.clear();
    entities.shrink_to_fit();
}