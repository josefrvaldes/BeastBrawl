#include "StateInGame.h"

#include <CLPhysics/CLPhysics.h>
#include <Components/CMesh.h>
#include <Components/CTexture.h>
#include <GameValues.h>

#include <chrono>
#include <iostream>

#include "../Components/CBoundingCilindre.h"
#include "../Components/CBoundingSphere.h"
#include "../Components/CShader.h"
#include "../Components/CTotem.h"
#include "../Constants.h"

using namespace std;
using namespace chrono;

StateInGame::StateInGame() {
    std::cout << "> INGAME constructor" << std::endl;

    // aunque physics es un sistema, no se llama desde InitializeSystems
    // porque tiene que estar inicializado antes de llamar a InitializeManagers
    cout << "------------------------------------------------------------------" << endl;
    cout << "delta vale" << Constants::DELTA_TIME << endl;
    cout << "------------------------------------------------------------------" << endl;
    //physics = make_unique<Physics>(Constants::DELTA_TIME);

    //cam = make_shared<Camera>(glm::vec3(100.0f, 0.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    ground = make_shared<GameObject>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "", "training_ground.obj");

    // Ordena las posiciones del ranking 1 vez antes de empezar la partida
}

StateInGame::~StateInGame() {
    cout << "Llamamos al destructor de StateInGame" << endl;
}


void StateInGame::InitializeFacades() {
    // Inicializamos las facadas
    inputEngine = InputFacadeManager::GetInstance()->GetInputFacade();
    physicsEngine = PhysicsFacadeManager::GetInstance()->GetPhysicsFacade();
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeSuscribeEvents();

    //Pantalla de carga
    renderEngine->FacadeInitResources();
}

void StateInGame::AddElementsToRender() {
    

    // Entidades iniciales
    renderEngine->FacadeAddObjectCar(manCars.get()->GetCar().get());  //Anyadimos el coche
    for (auto cars : manCars->GetEntities()) {                        // Anyadimos los coches
        if (manCars.get()->GetCar().get() != cars.get())
            renderEngine->FacadeAddObject(cars.get());
    }

    renderEngine->FacadeAddObject(ground.get());  //Anyadimos el suelo

    //Añadimos todos los power ups
    for (auto bpu : manBoxPowerUps->GetEntities())
        renderEngine->FacadeAddObject(bpu.get());

    renderEngine->FacadeAddCamera(manCamera.get()->getCamera());

    renderEngine->FacadeAddObjectTotem(manTotems->GetEntities()[0].get());

    //Añadimos las luces
    for (auto light : manLight->GetEntities()) {
        renderEngine->FacadeAddObject(light.get());
    }

    renderEngine->FacadeAddSkybox("media/skybox/right.jpg",
                                  "media/skybox/left.jpg",
                                  "media/skybox/top.jpg",
                                  "media/skybox/bottom.jpg",
                                  "media/skybox/front.jpg",
                                  "media/skybox/back.jpg");

    if (manLight->GetEntities().size() > 0) {
        auto lightWithShadow = manLight->GetEntities()[0];
        auto cId = static_cast<CId *>(lightWithShadow->GetComponent(CompType::IdComp).get());
        renderEngine->FacadeAddShadowMapping(cId->id);
    }

    //Añadimos las particulas al arbol
    for (auto particleSystem : manParticleSystem->GetEntities()) {
        renderEngine->FacadeAddObject(particleSystem.get());
    }

    //Añadimos los escudos al arbol
    for(auto shield : manShield->GetEntities()){
        renderEngine->FacadeAddObject(shield.get());
    }

    // Añadimos cosas a la fachada de render
    renderEngine->FacadeAddPlates(manNamePlates.get());
}

void StateInGame::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    cout << "Estamos inicializando el clPhysics" << endl;
    // NO ALTERAR EL ORDEN DEL ADD, QUE USO EL ORDEN PARA DISTINGUIR ENTRE MANAGERS!!!
    clPhysics = make_unique<CLPhysics>();
    clPhysics->AddManager(manCars);
    clPhysics->AddManager(manWall);
    clPhysics->AddManager(manOBB);
    clPhysics->AddManager(manGround);
    clPhysics->AddManager(manPowerUp);
    clPhysics->AddManager(manNavMesh);
    clPhysics->AddManager(manBoxPowerUp);
    clPhysics->AddManager(manTotem);
}

void StateInGame::InitializeSystemData(){
    systemDataVision = make_unique<SystemData>(clPhysics.get());

    systemDataVision->AddManager(*manCars.get());
    systemDataVision->AddManager(*manPowerUps.get());
    systemDataVision->AddManager(*manBoxPowerUps.get());
    systemDataVision->AddManager(*manTotems.get());
    systemDataVision->AddManager(*manWayPoint.get());
    systemDataVision->AddManager(*manNavMesh.get());
    systemDataVision->AddManager(*manBoundingWall.get());
    systemDataVision->AddManager(*manBoundingOBB.get());
    systemDataVision->AddManager(*manBoundingGround.get());

    manCars->AddSystemDataVision(systemDataVision.get());
}

void StateInGame::InitializeSystems(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    InitializeCLPhysics(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
    InitializeSystemData();
    // incializa el system physics PU, no hace falta más código para esto
    collisions = make_shared<Collisions>();
    sysBoxPowerUp = make_shared<SystemBoxPowerUp>();
    sysLoD = make_unique<SystemLoD>();
    sysRanking = make_unique<SystemRanking>();
    sysHud = make_unique<SysHud>();

    Car *mainCar = static_cast<Car *>(manCars.GetCar().get());
    Car *car = static_cast<Car *>(manCars.GetEntities()[Utils::getRandomInt(1, manCars.GetEntities().size() - 1)].get());
    Totem *totem = static_cast<Totem *>(manTotems->GetEntities()[0].get());
    sysAnimStart = make_unique<SystemAnimationStart>(manCamera->getCamera(), totem, mainCar, car);
    sysAnimEnd = make_unique<SystemAnimationEnd>(manCamera->getCamera());
    sysHurt = make_unique<SystemHurt>();
}

void StateInGame::InitializeManagers() {
    // inicializa el man PU, no hace falta más código para esto
    manCars = make_shared<ManCar>();
    StateInGame::CreateMainCar();
    manCamera = make_unique<ManCamera>(manCars->GetCar().get(), Constants::DELTA_TIME);
    manWayPoint = make_shared<ManWayPoint>();  //Se crean todos los waypoints y edges
    manPowerUps = make_shared<ManPowerUp>(manCars);
    manBoxPowerUps = make_shared<ManBoxPowerUp>();
    manBoundingWall = make_shared<ManBoundingWall>();
    manBoundingOBB = make_shared<ManBoundingOBB>();
    manBoundingGround = make_shared<ManBoundingGround>();
    manNavMesh = make_shared<ManNavMesh>();
    manTotems = make_shared<ManTotem>(manNavMesh.get());
    manNamePlates = make_shared<ManNamePlate>(manCars.get());
    manLight = make_shared<ManLight>();
    manGameRules = make_unique<ManGameRules>();
    manHudEvent = make_unique<ManHUDEvent>();
    manParticleSystem = make_unique<ManParticleSystem>();
    manShield = make_unique<ManShield>();

    managersEntities.emplace_back(manCars);
    managersEntities.emplace_back(manPowerUps);
    managersEntities.emplace_back(manBoxPowerUps);
    managersEntities.emplace_back(manTotems);

    // Es raro pero diria que aqui tengo que ir añadiendo sistemas de particulas
    // Añadimos las particulas a todas las cajas
    vector<string> puParticles;
    puParticles.push_back("media/particleRedStar.png");
    puParticles.push_back("media/particleYellowStar.png");
    puParticles.push_back("media/particleRedTriangle.png");
    puParticles.push_back("media/particleYellowTriangle.png");
    puParticles.push_back("media/particle_test.png");
    for (auto boxPowerUp : manBoxPowerUps->GetEntities()) {
        auto cId = static_cast<CId *>(boxPowerUp->GetComponent(CompType::IdComp).get());
        manParticleSystem->CreateParticleSystem(cId->id, glm::vec3(0.0f, 0.0f, 0.0f), 30, glm::vec3(200.0f, 400.0f, 200.0f), puParticles, 5, 5, 100, 30, 150, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0, 0x1 | 0x4, false, false);
    }

    // Añadimos sistema de particulas a los (el) totem
    vector<string> totemParticles;
    totemParticles.push_back("media/particle_test.png");
    for (auto totem : manTotems->GetEntities()) {
        auto cId = static_cast<CId *>(totem->GetComponent(CompType::IdComp).get());
        manParticleSystem->CreateParticleSystem(cId->id, glm::vec3(0.0f, 0.0f, 0.0f), 100, glm::vec3(0.0f, 50.0f, 0.0f), totemParticles, 5, 15, 100, 2, 5000, glm::vec3(30.0f, 0.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0, 0x4, true, true);
    }
}

//Carga los bancos de sonido InGame.
void StateInGame::InitState() {
    //Si la direccion de soundEngine!=0 es que viene del PAUSE, por lo que no deberia hacerlo.
    if (!soundEngine) {
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
        if (soundEngine) {
            soundEngine->SetState(4);
        }
    } else {
        soundEngine->ResumeAllEvent();
    }
}

void StateInGame::CreateMainCar() {
    if (manCars) {
        auto pj = GameValues::GetInstance()->GetCharacter();
        manCars->CreateMainCar(pj);
        //manNamePlates->CreateNamePlate(manCars->GetCar().get());
        /*auto cCar = static_cast<CCar*>(manCars->GetCar()->GetComponent(CompType::CarComp).get());
        if (cCar){
            cout << "PESO: " << cCar->weight << " - VELMAX: " << cCar->maxSpeed << " - ACELETARION: " << cCar->acceleration << "\n";
        }*/
    }
}

///////////////////////

///////////////////////

bool StateInGame::UpdateAnimationStart() {
    bool animationFinished = sysAnimStart->Animate();
    renderEngine->UpdateCamera(manCamera.get()->getCamera(), manCars.get());
    auto cCam = static_cast<CCamera *>(manCamera.get()->getCamera()->GetComponent(CompType::CameraComp).get());
    renderEngine->SetCamTarget(cCam->target);
    return animationFinished;
    // si hemos acabado la animación de inicio...
    // if (animationFinished) {
    //     GoToCountdownAnimation();
    // }
}

void StateInGame::UpdateAnimationCountdown() {
    int64_t now = Utils::getMillisSinceEpoch();
    int64_t interval = now - timerCountdown;
    if (interval > 1000 && timerCountdown > 0) {
        currentCountdown--;
        // cout << "Current countdown " << unsigned(currentCountdown) << endl;
        timerCountdown = Utils::getMillisSinceEpoch();
        if (currentCountdown == 0) {
            GoToUpdateGame();
        }
    }
}

void StateInGame::GoToUpdateGame() {
    currentUpdateState = UpdateState::GAME;
    manGameRules->ResetClock();
    EventManager::GetInstance().AddEventMulti(Event{EventType::START_MINGAME});
}

void StateInGame::UpdateAnimationEnd() {
    int64_t now = Utils::getMillisSinceEpoch();
    int64_t interval = now - timerEnd;
    // cout << "Estamos en UpdateAnimationEnd, timerEnd[" << timerEnd << "] now[" << now << "] interval[" << interval << "]" << endl;
    // si ha terminado la animación, salimos
    if (interval > 4000) {
        GoToStateEndrace();
        // si no ha terminado..
    } else {
        // si todavía no habíamos asignado un winner, es decir, es el primer frame, buscamos winner y se lo asignamos
        if(sysAnimEnd->GetWinner() == nullptr) {
            Car *winner = static_cast<Car*>(manCars->GetCurrentWinner());
            sysAnimEnd->SetWinner(winner);
        }
        // y ya animamos y demás
        sysAnimEnd->Animate();
        renderEngine->UpdateCamera(manCamera.get()->getCamera(), manCars.get());
        auto cCam = static_cast<CCamera *>(manCamera.get()->getCamera()->GetComponent(CompType::CameraComp).get());
        renderEngine->SetCamTarget(cCam->target);
    }
}

void StateInGame::UpdateGame() {
    EventManager &em = EventManager::GetInstance();
    em.Update();

    manNavMesh->Update(*(manCars.get()));

    // ACTUALIZACION DE LOS MANAGERS DE LOS COCHES
    bool gameFinished = manCars->UpdateCarPlayer(*(manTotems.get()));
    if (gameFinished) {
        GoToEndAnimation();
    }

    // ACTUALIZACION DE LAS FISICAS DE LOS COCHES
    //physics->update(manCars->GetCar().get());
    manCamera->Update();

    sysBoxPowerUp->update(manBoxPowerUps.get());

    clPhysics->Update(0.1666f);
    IntersectsCLPhysics();


    // Actualizaciones en Irrlich
    renderEngine->UpdateCamera(manCamera.get()->getCamera(), manCars.get());
    physicsEngine->UpdateCar(manCars.get()->GetCar().get(), manCamera.get()->getCamera());

    for (auto actualPowerUp : manPowerUps->GetEntities())  // actualizamos los powerUp en irrlich
        physicsEngine->UpdatePowerUps(actualPowerUp.get());

    renderEngine->FacadeUpdatePlates(manNamePlates.get());
    physicsEngine->UpdateTransformable(manTotems->GetEntities()[0].get());

    //Updates de los eventos de sonido
    soundEngine->UpdateCars(manCars->GetEntities());
    soundEngine->UpdatePowerUps(manPowerUps->GetEntities());
    soundEngine->UpdateTotem(manCars->GetCar(), manTotems->GetEntities());
    soundEngine->UpdateListener(manCars->GetCar());

    manTotems->Update();
    // al final de la ejecucion eliminamos todos los powerUps que se deben eliminar
    manPowerUps->Update();

    // Inicio LoD
    sysLoD->UpdateMeshes(manCars->GetEntities(), manCamera.get()->getCamera());
    sysLoD->UpdateMeshes(manPowerUps->GetEntities(), manCamera.get()->getCamera());
    sysLoD->UpdateMeshes(manTotems->GetEntities(), manCamera.get()->getCamera());
    renderEngine->FacadeUpdateMeshesLoD(manCars->GetEntities());
    renderEngine->FacadeUpdateMeshesLoD(manPowerUps->GetEntities());
    renderEngine->FacadeUpdateMeshesLoD(manTotems->GetEntities());
    // Fin LoD


    sysHurt->Update(manCars->GetEntities());

    // Inicio Animaciones
    // sysLoD->UpdateAnimations(manBoxPowerUps->GetEntities(), manCamera.get()->getCamera());
    // renderEngine->FacadeUpdateAnimationsLoD(manBoxPowerUps->GetEntities());
    // renderEngine->FacadeAnimate(manBoxPowerUps->GetEntities());
    // Fin Animaciones

    //Actualiza el ranking y los eventos de hud
    sysRanking->Update(manCars.get());
    sysHud->UpdateEventHud(manHudEvent.get());
    gameFinished = manGameRules->Update(manCars->GetEntities(), manTotems->GetEntities());
    if (gameFinished) {
        GoToEndAnimation();
    }

    if (Constants::CLIPPING_OCTREE) {
        octreeScene = make_unique<Octree>(glm::vec3(0.0, 500.0, 0.0), 700.0, managersEntities);
        octreeScene->UpdateVisibleObjects(renderEngine);
    }
}

void StateInGame::IntersectsCLPhysics(){
    clPhysics->IntersectsCarsPowerUps(*manCars.get(), *manPowerUps.get(), manNavMesh.get());
    clPhysics->IntersectCarsBoxPowerUp(*manCars.get(), *manBoxPowerUps.get());
    clPhysics->IntersectCarsTotem(*manCars.get(), *manTotems.get());
    clPhysics->IntersectPowerUpWalls(*manPowerUps.get(), *manBoundingWall.get(), *manBoundingOBB.get());
    clPhysics->IntersectTotemWalls(*manTotems.get(), *manBoundingWall.get(), *manBoundingOBB.get());
}

void StateInGame::Update() {
    switch (currentUpdateState) {
        case UpdateState::START:
            UpdateAnimationStart();
            break;
        case UpdateState::END:
            UpdateAnimationEnd();
            break;
        case UpdateState::GAME:
            UpdateGame();
            EventManager::GetInstance().AddEventMulti(Event{EventType::START_MINGAME});
            break;

        default:
            cout << "currentUpdateState inválido" << endl;
            break;
    }
}

void StateInGame::Render() {
    renderEngine->FacadeBeginScene();
    // renderEngine->FacadeDraw();  //Para dibujar primitivas debe ir entre el drawAll y el endScene
    renderEngine->FacadeDrawAll();

    if (Constants::CLIPPING_OCTREE && octreeScene.get())
        octreeScene->Draw(renderEngine);

    renderEngine->FacadeDrawHUD(manCars->GetCar().get(), manCars.get(), manGameRules->GetGlobalClock().get(), manHudEvent.get(), manGameRules.get());
    renderEngine->FacadeDrawGraphEdges(manWayPoint.get());
    if (currentUpdateState == UpdateState::COUNTDOWN) {
        // todo: esto de meter el width y el height aquí a piñón y los filenames.. es una kk
        const int fileWIDTH = 300;
        const int fileHEIGHT = 200;
        string fileName = "media/" + std::to_string(currentCountdown) + ".png";
        tuple<int, int> screenSize = renderEngine->GetScreenSize();
        int width = std::get<0>(screenSize);
        // int height = std::get<1>(screenSize);
        int posX = width / 2 - fileWIDTH / 2;
        int posY = 200;
        renderEngine->Draw2DImage(posX, posY, fileWIDTH, fileHEIGHT, 0.7, fileName, true);
    }
    // renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), isColliding);

    for (auto &actualPowerUp : manPowerUps->GetEntities()) {
        renderEngine->FacadeDrawBoundingBox(actualPowerUp.get(), false);
    }
    for (auto &wall : manBoundingWall->GetEntities()) {
        renderEngine->FacadeDrawBoundingPlane(wall.get());
    }
    for (auto &ground : manBoundingGround->GetEntities()) {
        renderEngine->FacadeDrawBoundingGround(ground.get());
    }
    for (auto &obb : manBoundingOBB->GetEntities()) {
        renderEngine->FacadeDrawBoundingOBB(obb.get());
    }

    renderEngine->FacadeDrawAIDebug(manCars.get(), manNavMesh.get(), manWayPoint.get());
    renderEngine->FacadeEndScene();
}

void StateInGame::GoToEndAnimation() {
    soundEngine->SetState(11);
    currentUpdateState = UpdateState::END;
    timerEnd = Utils::getMillisSinceEpoch();
    EventManager::GetInstance().AddEventMulti(Event{EventType::LAUNCH_ANIMATION_END_MULTI});
}

void StateInGame::GoToCountdownAnimation() {
    soundEngine->SetState(12);
    // ponemos como próximo state el countdown
    currentUpdateState = UpdateState::COUNTDOWN;
    cout << "Cambiamos a UpdateCountdown" << endl;
    // actualizamos la pos de la cámara para que se ponga detrás del coche principal
    manCamera->Update();
    renderEngine->UpdateCamera(manCamera.get()->getCamera(), manCars.get());

    // iniciamos el timer de countdown
    timerCountdown = Utils::getMillisSinceEpoch();
    // cout << "Current countdown " << unsigned(currentCountdown) << endl;
}

void StateInGame::GoToStateEndrace() {
    cout << "Vamos a lanzar un evento de STATE_ENDRACE desde StateInGame" << endl;
    EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_ENDRACE});
    EventManager::GetInstance().Update();
}