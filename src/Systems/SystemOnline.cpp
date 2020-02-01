#include "SystemOnline.h"
#include "../Constants.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Managers/ManCar.h"
#include "../Online/UDPClient.h"
#include "src/Components/COnline.h"
#include "src/Entities/CarHuman.h"

using namespace boost::asio;

SystemOnline::SystemOnline(ManCar &manCar_) : manCar{manCar_}, udpClient{make_unique<UDPClient>(SERVER_HOST, SERVER_PORT_UDP)} {
    shared_ptr<CarHuman> car = manCar.GetCar();
    // COnline *cOnline = static_cast<COnline *>(car->GetComponent(CompType::OnlineComp).get());
    idOnlineMainCar = 2;  // TODO quitar esto y que se haga de verdad, solo pruebas
}

void SystemOnline::SendInputs(vector<Constants::InputTypes> &inputs) {
    //udpClient->SendInputs(inputs);
    // if (inputs.size() > 0)
    udpClient->SendInputs(inputs, idOnlineMainCar);
}