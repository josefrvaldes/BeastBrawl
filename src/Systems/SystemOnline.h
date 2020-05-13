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
class PowerUp;
class COnline;

class SystemOnline {
   public:
    SystemOnline(ManCar &manCar_, uint16_t idOnlineMainCar_);
    ~SystemOnline();

    void Update();
    void SendInputs(const vector<Constants::InputTypes> &d, float speed, float wheelRotation, float skidDeg, float skidRotation) const;
    void SendSync(ManCar* manCars, ManTotem* manTotem) const;
    // void SendEndgame() const;
    void SendCatchPU(CPowerUp& cPowerUp) const;
    void SendThrowPU(const shared_ptr<PowerUp>& powerUp, const uint16_t idToPursue) const;
    void SendCrashPUCar(const uint16_t idPowerUp, const uint16_t idCar) const;
    void SendCrashPUWall(const uint16_t idPowerUp) const;
    void SendCatchTotem(uint16_t idCarCatched) const;
    void SendLostTotem(uint16_t idCarCatched, const glm::vec3 &position, float speedTotem, int rotationTotemY, int numNavMesh) const;
    void SendRoboJorobo() const;
    void SendWaitingForCountdown() const;
    void SendNitro(uint16_t idCarWithTotem, uint16_t idCarWithNitro) const;
    int64_t GetGameTime() const;
    uint16_t idOnlineMainCar;
    bool ClocksStartedSincing() {return clocksStartedSyncing;};
    void SyncClocks();

   private:
    bool CheckIfSyncFinished(COnline *cOnlineMainCar, const uint8_t numMeasurementsToCompare) const;
    void SendFinalClockSync(COnline *cOnlineMainCar) ;
    void SubscribeToEvents();
    void SyncNOW();
    void EventEndgame(DataMap *);
    void EventLaunchAnimationEnd(DataMap *);
    void NewClockSyncReceived(DataMap *);
    void NewFinalClockSyncReceived(DataMap *);

    bool clocksStartedSyncing{false};
    ManCar &manCar;
    unique_ptr<UDPClient> udpClient;

    const uint8_t TIMES_RESEND = 3;



    int64_t timeStartClock{-1};
    float turnout;
    const uint8_t MAX_NUM_MEASUREMENTS = 10;
    boost::asio::io_context context;
    unique_ptr<boost::asio::steady_timer> timer;

    const int64_t TIME_TO_WAIT_FOR_SYNC = 500;
};