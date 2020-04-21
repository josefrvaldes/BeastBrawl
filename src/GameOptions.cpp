#include "GameOptions.h"

GameOptions* GameOptions::instance = 0;

GameOptions* GameOptions::GetInstance() {
    if (instance == 0) {
        instance = new GameOptions();
    }
    return instance;
}