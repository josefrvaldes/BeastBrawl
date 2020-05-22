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
 * @author Clover Games Studio
 * 
 */
 
 
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
#include "../Components/CAnimation.h"
#include "../Components/CTotem.h"
#include "../Entities/Camera.h"
#include "../Constants.h"
#include "../GameValues.h"

using namespace std;
using namespace chrono;

StateInGame::StateInGame() {
    //std::cout << "> INGAME constructor" << std::endl;
    RenderFacadeManager::GetInstance()->GetRenderFacade()->FacadeReleaseSelectCharacter();
}

StateInGame::~StateInGame() {
    renderEngine->FacadeReleaseStateInGame();
}


void StateInGame::InitializeFacades() {
    // Inicializamos las facadas
    inputEngine = InputFacadeManager::GetInstance()->GetInputFacade();
    physicsEngine = PhysicsFacadeManager::GetInstance()->GetPhysicsFacade();
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeSuscribeEvents();

    //Pantalla de carga
    auto cCar = static_cast<CCar*>(manCars->GetCar()->GetComponent(CompType::CarComp).get());
    renderEngine->FacadeInitResources(cCar->character);
}

void StateInGame::AddElementsToRender() {
    

    // Entidades iniciales
    renderEngine->FacadeAddObjectCar(manCars.get()->GetCar().get());  //Anyadimos el coche
    for (auto cars : manCars->GetEntities()) {                        // Anyadimos los coches
        if (manCars.get()->GetCar().get() != cars.get())
            renderEngine->FacadeAddObject(cars.get());
    }

    // TODO: LOS SUELOS CON FRUSTUM
    for(const auto& currentPartGround : manGameRules->GetGrounds()){
        renderEngine->FacadeAddStaticObject(currentPartGround.get());
    }

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

    CreateVegetation();

    //Añadimos las particulas al arbol
    for (auto particleSystem : manParticleSystem->GetEntities()) {
        renderEngine->FacadeAddObject(particleSystem.get());
    }


    // Añadimos cosas a la fachada de render
    renderEngine->FacadeAddPlates(manNamePlates.get());


    // LOS ESCUDOS SIEMPRE AL FINAL POR SER TRANSPARENTES
    //Añadimos los escudos al arbol
    for(auto shield : manShield->GetEntities()){
        renderEngine->FacadeAddObject(shield.get());
    }
}

void StateInGame::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    //cout << "Estamos inicializando el clPhysics" << endl;
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
    manNavMesh = make_shared<ManNavMesh>();
    manTotems = make_shared<ManTotem>(manNavMesh.get());
    manSpawn = make_unique<ManSpawn>();
    manCars = make_shared<ManCar>(manSpawn->GetPositionsSpawn());
    StateInGame::CreateMainCar();
    manCamera = make_unique<ManCamera>(manCars->GetCar().get(), Constants::DELTA_TIME);
    manWayPoint = make_shared<ManWayPoint>();  //Se crean todos los waypoints y edges
    manPowerUps = make_shared<ManPowerUp>(manCars);
    manBoxPowerUps = make_shared<ManBoxPowerUp>();
    manBoundingWall = make_shared<ManBoundingWall>();
    manBoundingOBB = make_shared<ManBoundingOBB>();
    manBoundingGround = make_shared<ManBoundingGround>();
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
    puParticles.push_back("media/particleTriangleBrown.png");
    puParticles.push_back("media/particleStarMarron.png");
    puParticles.push_back("media/particleYellowStar.png");
    for (auto boxPowerUp : manBoxPowerUps->GetEntities()) {
        auto cId = static_cast<CId *>(boxPowerUp->GetComponent(CompType::IdComp).get());
        manParticleSystem->CreateParticleSystem(cId->id, glm::vec3(0.0f, 0.0f, 0.0f), 30, glm::vec3(200.0f, 400.0f, 200.0f), puParticles, 5, 5, 100, 30, 150, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0, 0x1 | 0x4, false, false);
    }

    // Añadimos sistema de particulas a los (el) totem
    vector<string> totemParticles;
    totemParticles.push_back("media/totemShiny.png");
    for (auto totem : manTotems->GetEntities()) {
        auto cId = static_cast<CId *>(totem->GetComponent(CompType::IdComp).get());
        manParticleSystem->CreateParticleSystem(cId->id, glm::vec3(0.0f, 0.0f, 0.0f), 100, glm::vec3(0.0f, 300.0f, 0.0f), totemParticles, 10, 12, 100, 2, 2000, glm::vec3(10.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0, 0x4, true, true);
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
            if(winner->HasComponent(CompType::AnimationComp)) {
                auto cAnimation = static_cast<CAnimation*>(winner->GetComponent(CompType::AnimationComp).get());
                cAnimation->ActivateAnimationWin();
                renderEngine->FacadeUpdateAnimationsLoD(manCars->GetEntities());
            }
            sysAnimEnd->SetWinner(winner);
        }
        // y ya animamos y demás
        sysAnimEnd->Animate();
        renderEngine->FacadeAnimate(manCars->GetEntities());
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



    sysBoxPowerUp->update(manBoxPowerUps.get());

    //auto posCar = static_cast<CTransformable *>(manCars->GetCar()->GetComponent(CompType::TransformableComp).get())->position;
    //cout <<" MI POSICON 1 ES : (" << posCar.x<< " , " << posCar.y<< " , " << posCar.z<< " )" << endl;

    clPhysics->Update(0.1666f);
    IntersectsCLPhysics();


    // ACTUALIZACION DE LAS FISICAS DE LOS COCHES
    manCamera->Update();


    // Actualizaciones en la fachada
    renderEngine->UpdateCamera(manCamera.get()->getCamera(), manCars.get());
    physicsEngine->UpdateCar(manCars.get()->GetCar().get(), manCamera.get()->getCamera());

    for (auto actualPowerUp : manPowerUps->GetEntities())  // actualizamos los powerUp en la fachada
        physicsEngine->UpdatePowerUps(actualPowerUp.get());

    for (auto actualBoxPowerUp : manBoxPowerUps->GetEntities())  // actualizamos los powerUp en la fachada
        physicsEngine->UpdatePowerUps(actualBoxPowerUp.get());

    renderEngine->FacadeUpdatePlates(manNamePlates.get(), manCars.get());
    physicsEngine->UpdateTransformable(manTotems->GetEntities()[0].get());


// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     timeStartSeccion = std::chrono::system_clock::now();
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    sysHurt->Update(manCars->GetEntities());
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      end = std::chrono::system_clock::now();
//      elapsed_millisecons = std::chrono::duration_cast<std::chrono::nanoseconds>
//                              (end-timeStartSeccion).count();
//     cout << "TIEMO ACTUAL ULDATE  (Hurt):  " << elapsed_millisecons/1000000 << endl;
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Updates de los eventos de sonido
    soundEngine->UpdateCars(manCars->GetEntities());
    soundEngine->UpdatePowerUps(manPowerUps->GetEntities());
    soundEngine->UpdateTotem(manCars->GetCar(), manTotems->GetEntities());
    soundEngine->UpdateListener(manCars->GetCar());

    manTotems->Update();
    // al final de la ejecucion eliminamos todos los powerUps que se deben eliminar
    manPowerUps->Update();

    // Inicio LoD
    // sysLoD->UpdateMeshes(manCars->GetEntities(), manCamera.get()->getCamera());
    // sysLoD->UpdateMeshes(manPowerUps->GetEntities(), manCamera.get()->getCamera());
    // sysLoD->UpdateMeshes(manTotems->GetEntities(), manCamera.get()->getCamera());
    // renderEngine->FacadeUpdateMeshesLoD(manCars->GetEntities());
    // renderEngine->FacadeUpdateMeshesLoD(manPowerUps->GetEntities());
    // renderEngine->FacadeUpdateMeshesLoD(manTotems->GetEntities());
    // Fin LoD

    sysHurt->Update(manCars->GetEntities());

    // Inicio Animaciones
    // sysLoD->UpdateAnimations(manBoxPowerUps->GetEntities(), manCamera.get()->getCamera());
    renderEngine->FacadeUpdateAnimationsLoD(manCars->GetEntities());
    renderEngine->FacadeAnimate(manCars->GetEntities());
    // Fin Animaciones

    //Actualiza el ranking y los eventos de hud
    sysHud->UpdateEventHud(manHudEvent.get());
    gameFinished = manGameRules->Update(manCars->GetEntities(), manTotems->GetEntities());
    sysRanking->Update(manCars.get());
    if (gameFinished) {
        GoToEndAnimation();
    }

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      end = std::chrono::system_clock::now();
//      elapsed_millisecons = std::chrono::duration_cast<std::chrono::nanoseconds>
//                              (end-timeStartSeccion).count();
//     cout << "TIEMO ACTUAL ULDATE  (RANKING):  " << elapsed_millisecons/1000000 << endl;
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     timeStartSeccion = std::chrono::system_clock::now();
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    if (Constants::CLIPPING_OCTREE) {
                        octreeScene = make_unique<Octree>(glm::vec3(0.0, 500.0, 0.0), 700.0, managersEntities);
                        octreeScene->UpdateVisibleObjects(renderEngine);
                    }
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      end = std::chrono::system_clock::now();
//      elapsed_millisecons = std::chrono::duration_cast<std::chrono::nanoseconds>
//                              (end-timeStartSeccion).count();
//     cout << "TIEMO ACTUAL ULDATE  (CLIPPING_OCTREE):  " << elapsed_millisecons/1000000 << endl;
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

void StateInGame::IntersectsCLPhysics(){
    clPhysics->IntersectsCarsPowerUps(*manCars.get(), *manPowerUps.get(), manNavMesh.get());
    clPhysics->IntersectCarsBoxPowerUp(*manCars.get(), *manBoxPowerUps.get());
    clPhysics->IntersectCarsTotem(*manCars.get(), *manTotems.get());
    clPhysics->IntersectPowerUpWalls(*manPowerUps.get(), *manBoundingWall.get(), *manBoundingOBB.get());
    clPhysics->IntersectTotemWalls(*manTotems.get(), *manBoundingWall.get(), *manBoundingOBB.get());
    clPhysics->IntersectCameraWalls(manCamera->getCamera(), manCamera->getPlayerFollow(), *manBoundingWall.get(), *manBoundingOBB.get());
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
            //cout << "currentUpdateState inválido" << endl;
            break;
    }
}

void StateInGame::Render() {
    renderEngine->FacadeBeginScene();
    // renderEngine->FacadeDraw();  //Para dibujar primitivas debe ir entre el drawAll y el endScene
    renderEngine->FacadeDrawAll();


    if (Constants::CLIPPING_OCTREE && octreeScene.get())
        octreeScene->Draw(renderEngine);

    renderEngine->FacadeDrawGraphEdges(manWayPoint.get());
    if (currentUpdateState == UpdateState::COUNTDOWN) {
        // todo: esto de meter el width y el height aquí a piñón y los filenames.. es una kk
        const int fileWIDTH = 300;
        const int fileHEIGHT = 300;
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
    
    auto showHUD = renderEngine->FacadeCheckShowHUD();
    if (showHUD) {
        if(currentUpdateState == UpdateState::GAME)
            renderEngine->FacadeDrawHUD(manCars->GetCar().get(), manCars.get(), manGameRules->GetGlobalClock().get(), manHudEvent.get(), manGameRules.get(), true); 
        else
            renderEngine->FacadeDrawHUD(manCars->GetCar().get(), manCars.get(), manGameRules->GetGlobalClock().get(), manHudEvent.get(), manGameRules.get(), false); 
        
  
    }
    renderEngine->FacadeEndScene();
}

void StateInGame::GoToEndAnimation() {
    soundEngine->SetState(11);
    sysRanking->Update(manCars.get());
    currentUpdateState = UpdateState::END;
    timerEnd = Utils::getMillisSinceEpoch();
    EventManager::GetInstance().AddEventMulti(Event{EventType::LAUNCH_ANIMATION_END_MULTI});
}

void StateInGame::GoToCountdownAnimation() {
    soundEngine->SetState(12);
    // ponemos como próximo state el countdown
    currentUpdateState = UpdateState::COUNTDOWN;
    //cout << "Cambiamos a UpdateCountdown" << endl;
    // actualizamos la pos de la cámara para que se ponga detrás del coche principal
    manCamera->Update();
    renderEngine->UpdateCamera(manCamera.get()->getCamera(), manCars.get());

    // iniciamos el timer de countdown
    timerCountdown = Utils::getMillisSinceEpoch();
    // cout << "Current countdown " << unsigned(currentCountdown) << endl;
}

void StateInGame::GoToStateEndrace() {
    if(!inputEngine->InTournament()){
        //cout << "Vamos a lanzar un evento de STATE_ENDRACE desde StateInGame" << endl;
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_ENDRACE});
    }else{
        //cout << "Vamos a lanzar un evento de STATE_ENDTOURNAMENT desde StateInGame" << endl;
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_ENDTOURNAMENT});
    }

    EventManager::GetInstance().Update();
}


// genera la vegetacion  ToDo: pasar a cargador de mapas
void StateInGame::CreateVegetation(){
    //renderEngine->FacadeAddGrass(200.0, glm::vec3(400.0f, 0.0f, 0.0f), glm::vec3(15.0, 15.0, 15.0), false);
    
    // Lateral 1 Abajo 
    renderEngine->FacadeAddGrass(250.0, 200.0, glm::vec3(530.0f, 0.0f, -504.0f), glm::vec3(15.0, 15.0, 15.0), true);
    renderEngine->FacadeAddGrass(50.0, 200.0, glm::vec3(374.0f, 0.0f, -510.0f), glm::vec3(5.0, 5.0, 5.0), true);

    // Lateral 2 Abajo
    renderEngine->FacadeAddGrass(300.0, 120.0, glm::vec3(500.0f, 0.0f, 501.0f), glm::vec3(20.0, 20.0, 20.0), true);
    renderEngine->FacadeAddGrass(300.0, 40.0, glm::vec3(500.0f, 0.0f, 420.0f), glm::vec3(5.0, 5.0, 5.0), true);
    renderEngine->FacadeAddGrass(20.0, 160.0, glm::vec3(340.0f, 0.0f, 481.0f), glm::vec3(5.0, 5.0, 5.0), true);

    // Lateral 3 Abajo
    renderEngine->FacadeAddGrass(60.0, 160.0, glm::vec3(-405.0f, 0.0f, -504.0f), glm::vec3(5.0, 5.0, 5.0), true);
    renderEngine->FacadeAddGrass(80.0, glm::vec3(-616.0f, 0.0f, -429.0f), glm::vec3(10.0, 10.0, 10.0), true);

    // Lateral 4 Abajo
    renderEngine->FacadeAddGrass(60.0, 160.0, glm::vec3(-405.0f, 0.0f, 504.0f), glm::vec3(5.0, 5.0, 5.0), true);
    renderEngine->FacadeAddGrass(220.0, 60.0, glm::vec3(-550.0f, 0.0f, 441.0f), glm::vec3(5.0, 5.0, 5.0), true);

    // Circulo 1 Abajo / 2 rayas
    renderEngine->FacadeAddGrass(250.0, glm::vec3(0.0f, 0.0f, -531.0f), glm::vec3(5.0, 5.0, 5.0), true);
    renderEngine->FacadeAddGrass(40.0, 610.0, glm::vec3(320.0f, 0.0f, -890.0f), glm::vec3(5.0, 5.0, 5.0), true);
    renderEngine->FacadeAddGrass(40.0, 610.0, glm::vec3(-340.0f, 0.0f, -890.0f), glm::vec3(5.0, 5.0, 5.0), true);

    // Debajo puente / 2 circulos abajo
    renderEngine->FacadeAddGrass(690.0, 70.0, glm::vec3(-10.0f, 0.0f, 1158.0f), glm::vec3(5.0, 5.0, 5.0), true);
    renderEngine->FacadeAddGrass(100.0, glm::vec3(69.0f, 0.0f, 213.0f), glm::vec3(5.0, 5.0, 5.0), true);
    renderEngine->FacadeAddGrass(100.0, glm::vec3(1.0f, 0.0f, 312.0f), glm::vec3(5.0, 5.0, 5.0), true);


    // Arriba 1
    renderEngine->FacadeAddGrass(220.0, 110.0, glm::vec3(-530.0f, 100.0f, 700.0f), glm::vec3(5.0, 5.0, 5.0), true);

    // Arriba 2
    renderEngine->FacadeAddGrass(180.0, glm::vec3(655.0f, 100.0f, 1124.0f), glm::vec3(6.0, 6.0, 6.0), true);
    renderEngine->FacadeAddGrass(20.0, 560.0, glm::vec3(941.0f, 100.0f, 887.0f), glm::vec3(5.0, 5.0, 5.0), true);

    // Arriba 3
    renderEngine->FacadeAddGrass(520.0, 180.0, glm::vec3(-695.0f, 100.0f, -1117.0f), glm::vec3(5.0, 5.0, 5.0), true);

    // Arriba 4
    renderEngine->FacadeAddGrass(50.0, 45.0, glm::vec3(410.0f, 100.0f, -775.0f), glm::vec3(10.0, 10.0, 10.0), true);
    renderEngine->FacadeAddGrass(50.0, 45.0, glm::vec3(410.0f, 100.0f, -1015.0f), glm::vec3(10.0, 10.0, 10.0), true);
}