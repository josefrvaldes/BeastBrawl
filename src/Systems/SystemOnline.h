#pragma once

#include <iostream>
#include "../Aliases.h"
#include "../Constants.h"
#include "../Online/UDPClient.h"
using namespace std;

class ManCar;

class SystemOnline {
   public:
    SystemOnline(ManCar &manCar_);
    ~SystemOnline(){};

    void Update();
    void SendInputs(vector<Constants::InputTypes> &d);
    uint16_t idOnlineMainCar;

   private:
    ManCar &manCar;
    unique_ptr<UDPClient> udpClient;
};