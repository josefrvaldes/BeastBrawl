#include "SystemOnline.h"
#include "../Constants.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Managers/ManCar.h"
#include "../Online/UDPClient.h"

SystemOnline::SystemOnline(ManCar &manCar_) : manCar{manCar_}, udpClient{make_unique<UDPClient>(SERVER_HOST, SERVER_PORT)} {
    SubscribeToEvents();
}

void SystemOnline::SubscribeToEvents() {
    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::PRESS_I,
        bind(&SystemOnline::PressUp, this, placeholders::_1),
        "AccelerateCar"));

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::PRESS_O,
        bind(&SystemOnline::PressDown, this, placeholders::_1),
        "Decelerate"));

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::PRESS_A,
        bind(&SystemOnline::PressLeft, this, placeholders::_1),
        "TurnLeftCar"));

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::PRESS_D,
        bind(&SystemOnline::PressRight, this, placeholders::_1),
        "TurnRightCar"));
}

void SystemOnline::PressUp(DataMap d) {
    udpClient.get()->SendInput(UDPClient::InputType::UP);
}

void SystemOnline::PressDown(DataMap d) {
    udpClient.get()->SendInput(UDPClient::InputType::DOWN);
}

void SystemOnline::PressLeft(DataMap d) {
    udpClient.get()->SendInput(UDPClient::InputType::LEFT);
}

void SystemOnline::PressRight(DataMap d) {
    udpClient.get()->SendInput(UDPClient::InputType::RIGHT);
}