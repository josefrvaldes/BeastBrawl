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
    sysOnline->SendInputs(inputs, 0.f);
    sysAnimStart->ResetTimer();
    sysRanking->Update(manCars.get());
}

StateInGameMulti::~StateInGameMulti() {
    cout << "Llamamos al destructor de StateInGameMulti" << endl;
}

void StateInGameMulti::InitState() {
    StateInGame::InitState();
}

void StateInGameMulti::InitCarHumans(const uint16_t idOnline_, const vector<uint16_t> arrayIdEnemies, const vector<uint8_t> characters) {
    // a este le llegan los coches
    //std::cout << "POR FIIIIIIIIIIIIIIIIIIIIIIIN: " << std::endl;
    //vector<uint16_t> arrayIdEnemies = IdPlayersOnline;
    vec3 posIniciales[] = {
        vec3(120.0f, 10.0f, -300.0f),
        vec3(20.0f, 10.0f, -300.0f),
        vec3(40.0f, 10.0f, -150.0f),
        vec3(-50.0f, 10.0f, -50.0f),
        vec3(50.0f, 10.0f, -200.0f),
        vec3(0.0f, 10.0f, 0.0f)};

    auto idComp = static_cast<CId *>(manCars->GetCar()->GetComponent(CompType::IdComp).get());
    auto cTransformable = static_cast<CTransformable *>(manCars->GetCar()->GetComponent(CompType::TransformableComp).get());
    cTransformable->position = posIniciales[idOnline_ - 1];
    COnline *cOnline = static_cast<COnline *>(manCars->GetCar()->GetComponent(CompType::OnlineComp).get());
    cOnline->idClient = idOnline_;
    manShield->CreateShield(idComp->id, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.5f));

    // for (auto idEnemy : arrayIdEnemies) {
    for (size_t i = 0; i < arrayIdEnemies.size(); i++) {
        uint16_t idEnemy = arrayIdEnemies[i];
        uint8_t currentCharacter = characters[i];
        vec3 pos = posIniciales[idEnemy - 1];

        //Le paso el PERSONAJE: ahora mismo a piñon
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

        manShield->CreateShield(idComp->id, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.5f));

        // esto era de cuando íbamos a hacer el buffer circular que al final se descartó
        shared_ptr<CBufferOnline> buffer = make_shared<CBufferOnline>();
        // BuffElement elem(cTransformable->position, cTransformable->rotation);
        // buffer->elems.push_back(elem);
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
        // const vector<Constants::InputTypes> &inputs = renderEngine->FacadeCheckInputMulti();
        const vector<Constants::InputTypes> &inputs = inputEngine->CheckInputMulti();
        time_point<system_clock> now = system_clock::now();
        auto millisSinceLastInputSent = duration_cast<milliseconds>(now - lastTimeSentInputs).count();
        if (millisSinceLastInputSent > 66 || previousInputs != inputs) {  // 100 = 10fps; 66 = 15fps   1000 = 60fps
            CCar* cCar = static_cast<CCar*>(manCars->GetCar()->GetComponent(CompType::CarComp).get());
            lastTimeSentInputs = now;
            previousInputs = inputs;
            sysOnline->SendInputs(inputs, cCar->speed);
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