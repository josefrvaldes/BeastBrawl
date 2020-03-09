#pragma once

#include <iostream>
#include "../Aliases.h"
#include "../Constants.h"
#include "../Online/UDPClient.h"
#include "../Components/CPowerUp.h"
using namespace std;

class ManCar;
class CPowerUp;
class ManTotem;

class SystemOnline {
   public:
    SystemOnline(ManCar &manCar_, uint16_t idOnlineMainCar_);
    ~SystemOnline(){};

    void Update();
    void SendInputs(const vector<Constants::InputTypes> &d) const;
    void SendSync(ManCar* manCars, ManTotem* manTotem) const;
    void SendEndgame() const;
    void SendCatchPU(CPowerUp& cPowerUp) const;
    void SendCatchTotem(uint16_t idCarCatched) const;
    void SendLostTotem(uint16_t idCarCatched, const glm::vec3 &position, int numNavMesh) const;
    uint16_t idOnlineMainCar;

   private:
    void SubscribeToEvents();
    void EventEndgame(DataMap *);

    ManCar &manCar;
    unique_ptr<UDPClient> udpClient;
};