#include "SystemOnline.h"
#include "../Constants.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Managers/ManCar.h"
#include "../Online/UDPClient.h"
#include "src/Components/COnline.h"
#include "src/Entities/CarHuman.h"

using namespace boost::asio;

SystemOnline::SystemOnline(ManCar &manCar_, uint16_t idOnlineMainCar_) : idOnlineMainCar{idOnlineMainCar_}, manCar{manCar_}, udpClient{make_unique<UDPClient>(SERVER_HOST, SERVER_PORT_UDP)} {
    shared_ptr<CarHuman> car = manCar.GetCar();
}

void SystemOnline::SendInputs(vector<Constants::InputTypes> &inputs) {
    udpClient->SendInputs(inputs, idOnlineMainCar);
}