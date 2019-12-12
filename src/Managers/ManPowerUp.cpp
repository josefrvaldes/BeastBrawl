#include "./ManPowerUp.h"
#include <iostream>
#include "../Entities/PowerUp.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"

class Position;
using namespace std;


ManPowerUp::ManPowerUp() {
    SubscribeToEvents();
    //CreatePowerUp(glm::vec3(30.0, 30.0, 30.0));
    cout << "Hemos creado el manager de powerup, ahora tenemos " << endl;
}

ManPowerUp::~ManPowerUp() {
    cout << "Llamando al destructor de manpowerup" << endl;
    PowerUps.clear();
    PowerUps.shrink_to_fit();
}




void ManPowerUp::CreatePowerUp(Data d) {
    shared_ptr<PowerUp> p = make_shared<PowerUp>(d.posCocheSalida->position, d.posCocheSalida->rotation, d.typePowerUp);
    PowerUps.push_back(p);

    // To-Do: crear el powerUp en irrlich
    // To-Do: hacer un update constante de los powrUp en el archivo renderFachadeIrrlich igual que tenemos para los coches
}



void ManPowerUp::SubscribeToEvents() {
    // lo ejecuta el coche al tirar power up
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PowerUp_Create,
        bind(&ManPowerUp::CreatePowerUp, this, placeholders::_1),
        "CreatePowerUp"));
}

