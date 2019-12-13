#include "./ManPowerUp.h"
#include <iostream>
#include "../Entities/PowerUp.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Aliases.h"
#include "../Facade/Render/RenderFacadeManager.h"

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




void ManPowerUp::CreatePowerUp(DataMap d) {
    typeCPowerUp type = any_cast<typeCPowerUp>(d["typePowerUp"]);
    CTransformable *transforSalida = any_cast<CTransformable *>(d["posCocheSalida"]);
    CTransformable *transforPerse = any_cast<CTransformable *>(d["posCochePerseguir"]);
    shared_ptr<PowerUp> powerUp = make_shared<PowerUp>(transforSalida->position, transforSalida->rotation, type, transforPerse);
    PowerUps.push_back(powerUp);

    shared_ptr<RenderFacadeManager> renderFacadeManager = RenderFacadeManager::GetInstance();
    shared_ptr<RenderFacade> renderEngine = renderFacadeManager->GetRenderFacade();
    renderEngine->FacadeAddObject(powerUp.get());
}



void ManPowerUp::SubscribeToEvents() {
    // lo ejecuta el coche al tirar power up
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PowerUp_Create,
        bind(&ManPowerUp::CreatePowerUp, this, placeholders::_1),
        "CreatePowerUp"));
}

