#pragma once

#include <iostream>
#include "../Aliases.h"
#include "../Online/UDPClient.h"
using namespace std;

class ManCar;

class SystemOnline {
   public:
    SystemOnline(ManCar &manCar_);
    ~SystemOnline(){};

    void Update();

   private:
    void SubscribeToEvents();
    void PressUp(DataMap d);
    void PressDown(DataMap d);
    void PressLeft(DataMap d);
    void PressRight(DataMap d);

    ManCar &manCar;
    unique_ptr<UDPClient> udpClient;
};