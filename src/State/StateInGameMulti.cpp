/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Antonio Jose Martinez Garcia
 * @author Jose Valdés Sirvent
 * 
 * 
 */


#include "StateInGameMulti.h"

#include <Components/CBufferOnline.h>
#include <Components/CTotem.h>

#include "../CLPhysics/CLPhysics.h"
#include "../Components/COnline.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Systems/SystemOnline.h"
#include "../Systems/Utils.h"

StateInGameMulti::StateInGameMulti(uint16_t idOnline_, const vector<uint16_t> idsEnemies_, const vector<uint8_t> characters_) : StateInGame() {
    
    // Reinicializar elementos
    RenderFacadeManager::GetInstance()->GetRenderFacade()->SetNumEnemyCars(0);
    auto cId = make_shared<CId>();
    cId->ResetNumIds();
    auto cNavMesh = make_shared<CNavMesh>();
    cNavMesh->ResetNumIds();
    shared_ptr<DataMap> data = make_shared<DataMap>();
    (*data)[TYPE_POWER_UP] = typeCPowerUp::None;
    RenderFacadeManager::GetInstance()->GetRenderFacade()->FacadeUpdatePowerUpHUD(data.get());


    GameValues::GetInstance()->SetGameTime(180);
    GameValues::GetInstance()->SetTimeTotem(30);
    InitState();
    InitializeManagers();
    InitCarHumans(idOnline_, idsEnemies_, characters_);
    InitializeSystems(*manCars.get(), *manBoundingWall.get(), *manBoundingOBB.get(), *manBoundingGround.get(), *manPowerUps.get(), *manNavMesh.get(), *manBoxPowerUps.get(), *manTotems.get());
    sysOnline = make_unique<SystemOnline>(*manCars.get(), idOnline_);
    manCars->SetSystemOnline(sysOnline.get());
    manTotems->SetSystemOnline(sysOnline.get());
    manPowerUps->SetSystemOnline(sysOnline.get());
    clPhysics->SetSystemOnline(sysOnline.get());
    InitializeFacades();

    AddElementsToRender();

    SubscribeToEvents();

    const vector<Constants::InputTypes> inputs;
    sysOnline->SendInputs(inputs, 0.f, 0.f, 0.f, 0.f);
    sysAnimStart->ResetTimer();

    //Inicializa el ranking y el minimapa
    sysRanking->Update(manCars.get());
    manGameRules->InitializeMiniMap(manCars->GetEntities(), manTotems->GetEntities());
}

StateInGameMulti::~StateInGameMulti() {
    cout << "Llamamos al destructor de StateInGameMulti" << endl;
}

void StateInGameMulti::InitState() {
    StateInGame::InitState();
}

void StateInGameMulti::InitCarHumans(const uint16_t idOnline_, const vector<uint16_t> arrayIdEnemies, const vector<uint8_t> characters) {
    cout << "El idOnline del mainCar en esta partida es " << idOnline_ << endl;
    auto idComp = static_cast<CId *>(manCars->GetCar()->GetComponent(CompType::IdComp).get());
    
    vec3 newPos = manCars->GetPosSpawn(idOnline_ - 1);
    manCars->GetCar()->SetPosition(newPos);
    manCars->GetCar()->SetRotation(glm::vec3(0, manCars->GetAngleToTotem(newPos),0));

    COnline *cOnline = static_cast<COnline *>(manCars->GetCar()->GetComponent(CompType::OnlineComp).get());
    cOnline->idClient = idOnline_;
    manShield->CreateShield(idComp->id, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(3.0f));

    for (size_t i = 0; i < arrayIdEnemies.size(); i++) {
        uint16_t idEnemy = arrayIdEnemies[i];
        uint8_t currentCharacter = characters[i];
        vec3 pos = manCars->GetPosSpawn(idEnemy - 1);//posIniciales[idEnemy - 1];

        manCars->CreateHumanCar(currentCharacter, pos);
        shared_ptr<Entity> car = manCars->GetEntities()[manCars->GetEntities().size() - 1];
        COnline *cOnline = static_cast<COnline *>(car->GetComponent(CompType::OnlineComp).get());
        cOnline->idClient = idEnemy;

        auto idComp = static_cast<CId *>(car->GetComponent(CompType::IdComp).get());
        string nameEvent = "Coche/motores";
        SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(idComp->id, pos, nameEvent, 1, 0);
        nameEvent = "PowerUp/escudo";
        SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(idComp->id, pos, nameEvent, 0, 0);
        nameEvent = "PowerUp/escudo_roto";
        SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(idComp->id, pos, nameEvent, 0);
        nameEvent = "PowerUp/choque_powerup";
        SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(idComp->id, pos, nameEvent, 0);
        nameEvent = "Coche/choque";
        SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(idComp->id, pos, nameEvent, 0);

        manShield->CreateShield(idComp->id, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(3.0f));

        shared_ptr<CBufferOnline> buffer = make_shared<CBufferOnline>();
        car->AddComponent(buffer);
    }

    for(auto car : manCars->GetEntities()){
        if(manCars->GetCar().get() != car.get()){
            manNamePlates->CreateNamePlate(car.get());
        }
    }
}

void StateInGameMulti::Input() {
    if (currentUpdateState == UpdateState::GAME) {
        const vector<Constants::InputTypes> &inputs = inputEngine->CheckInputMulti();
        time_point<system_clock> now = system_clock::now();
        auto millisSinceLastInputSent = duration_cast<milliseconds>(now - lastTimeSentInputs).count();
        if (millisSinceLastInputSent > 66 || previousInputs != inputs) {  // 100 = 10fps; 66 = 15fps   1000 = 60fps
            CCar* cCar = static_cast<CCar*>(manCars->GetCar()->GetComponent(CompType::CarComp).get());
            lastTimeSentInputs = now;
            previousInputs = inputs;
            sysOnline->SendInputs(inputs, cCar->speed, cCar->wheelRotation, cCar->skidDeg, cCar->skidRotation);
        }

        auto millisSinceLastSyncSent = duration_cast<milliseconds>(now - lastTimeSentSync).count();
        if (millisSinceLastSyncSent > 200) {  // 1000ms = 1s = 60fps; 2s = 120frames
            lastTimeSentSync = now;
            sysOnline->SendSync(manCars.get(), manTotems.get());
        }
    }
}

void StateInGameMulti::GoToUpdateGame() {
    StateInGame::GoToUpdateGame();
}

void StateInGameMulti::UpdateAnimationEnd() {
    StateInGame::UpdateAnimationEnd();
}

bool StateInGameMulti::UpdateAnimationStart() {
    bool animationFinished = StateInGame::UpdateAnimationStart();
    if(animationFinished)
        currentUpdateState = UpdateState::WAITING_FOR_COUNTDOWN;
    return animationFinished;
}

void StateInGameMulti::UpdateWaitingForCountdown() {
    if(!readyToCountdown) {
        manCamera->Update();
        renderEngine->UpdateCamera(manCamera.get()->getCamera(), manCars.get());
        cout << "Enviamos mensaje de waiting for countdown\n";
        readyToCountdown = true;
        sysOnline->SendWaitingForCountdown();
    }
    EventManager::GetInstance().Update();
}

void StateInGameMulti::UpdateAnimationCountdown() {
    StateInGame::UpdateAnimationCountdown();
    if(!sysOnline->ClocksStartedSincing())
        sysOnline->SyncClocks();
    EventManager::GetInstance().Update();
}

void StateInGameMulti::UpdateGame() {
    StateInGame::UpdateGame();

    for (auto actualCar : manCars->GetEntities()) {
        if (actualCar.get() != manCars->GetCar().get()) {
            //manNavMesh->UpdateNavMeshHuman(actualCar.get());  // actualizamos el navemesh en el que se encuentra al human
            // funcion para recibir los inputs del servidor, otra para enviar los nuestros, crear componente de input
            bool gameFinished = manCars->UpdateCarHuman(actualCar.get(), manTotems.get());
            if (gameFinished)
                GoToEndAnimation();

            physicsEngine->UpdateTransformable(actualCar.get());
        }
    }

    //ACTUALIZAR ESCUDOS
    physicsEngine->UpdateShields(manCars->GetEntities(), manShield->GetEntities());

}

void StateInGameMulti::Update() {
    switch (currentUpdateState) {
        case UpdateState::START:
            UpdateAnimationStart();
            break;
        case UpdateState::COUNTDOWN:
            UpdateAnimationCountdown();
            break;
        case UpdateState::WAITING_FOR_COUNTDOWN:
            UpdateWaitingForCountdown();
            break;
        case UpdateState::END:
            UpdateAnimationEnd();
            break;
        case UpdateState::GAME:
            UpdateGame();
            break;

        default:
            cout << "currentUpdateState inválido" << endl;
            break;
    }
}

void StateInGameMulti::Render() {
    renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), true);
    StateInGame::Render();
}

void StateInGameMulti::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    StateInGame::InitializeCLPhysics(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
}

void StateInGameMulti::InitializeManagers() {
    StateInGame::InitializeManagers();
}

void StateInGameMulti::InitializeSystems(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    StateInGame::InitializeSystems(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
}

void StateInGameMulti::InitializeFacades() {
    StateInGame::InitializeFacades();
}

void StateInGameMulti::AddElementsToRender() {
    StateInGame::AddElementsToRender();
}

void StateInGameMulti::SubscribeToEvents() {
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_LAUNCH_ANIMATION_END_RECEIVED,
        bind(&StateInGameMulti::GoToEndAnimationFromMulti, this, std::placeholders::_1),
        "Received end animation"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_LAUNCH_COUNTDOWN_ANIMATION_RECEIVED,
        bind(&StateInGameMulti::GoToCountdownAnimationFromMulti, this, std::placeholders::_1),
        "Received end animation"));
}

void StateInGameMulti::GoToEndAnimationFromMulti(DataMap *dataMap) {
    if (currentUpdateState != UpdateState::END) {
        uint16_t idWinner = any_cast<uint16_t>((*dataMap)[DataType::ID_WINNER]);
        for (auto entity : manCars->GetEntities()) {
            auto cOnline = static_cast<COnline *>(entity->GetComponent(CompType::OnlineComp).get());
            if (cOnline->idClient == idWinner) {
                Car *winner = static_cast<Car *>(entity.get());
                sysAnimEnd->SetWinner(winner);
                timerEnd = Utils::getMillisSinceEpoch();
                soundEngine->SetState(11);
                currentUpdateState = UpdateState::END;
                return;
            }
        }
    }
}

void StateInGameMulti::GoToCountdownAnimationFromMulti(DataMap *dataMap) {
    if (currentUpdateState != UpdateState::COUNTDOWN) {
        GoToCountdownAnimation();
    }
}