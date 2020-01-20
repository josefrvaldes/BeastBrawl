#include "SystemOnline.h"
#include "../Constants.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Managers/ManCar.h"
#include "../Online/UDPClient.h"

using namespace boost::asio;

SystemOnline::SystemOnline(ManCar &manCar_) : manCar{manCar_}, udpClient{make_unique<UDPClient>(SERVER_HOST, SERVER_PORT)} {
}

void SystemOnline::SendInputs(vector<Constants::InputTypes> &inputs) {
    //udpClient->SendInputs(inputs);
    if (inputs.size() > 0)
        udpClient->SendInput(inputs[0]);
}