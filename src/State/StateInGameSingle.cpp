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
 
 
 #include "StateInGameSingle.h"

#include "../Managers/ManAI.h"
#include <Components/CBrainAI.h>

StateInGameSingle::StateInGameSingle() : StateInGame() {
    InitState();
    InitializeManagers();
    InitCarAIS(*manCars, *manWayPoint);

    
    // posicionamos el mainCar. Antes estaba en StateInGame pero esto rompía el online por el random
    vec3 newPos = manCars->GetPosSpawn();
    manCars->GetCar()->SetPosition(newPos);
    // y lo ponemos mirando al totem, que antes no estaba
    manCars->GetCar()->SetRotation(glm::vec3(0, manCars->GetAngleToTotem(newPos),0));
    

    InitializeSystems(*manCars.get(), *manBoundingWall.get(), *manBoundingOBB.get(), *manBoundingGround.get(), *manPowerUps.get(), *manNavMesh.get(), *manBoxPowerUps.get(), *manTotems.get());
    InitializeFacades();

    AddElementsToRender();

    createSystemAI();
    sysAnimStart->ResetTimer();
    
    //Inicializa el ranking y el minimapa
    sysRanking->Update(manCars.get());
    manGameRules->InitializeMiniMap(manCars->GetEntities(), manTotems->GetEntities());
}

StateInGameSingle::~StateInGameSingle() {}

void StateInGameSingle::InitState() {
    StateInGame::InitState();
}

void StateInGameSingle::Input() {
    if (currentUpdateState == UpdateState::GAME) {
        renderEngine->FacadeCheckInputSingle();
        goingToPause = inputEngine->CheckInputSingle();   
    } else if(currentUpdateState == UpdateState::START || currentUpdateState == UpdateState::END) {
        bool spacePressed = inputEngine->CheckInputAnimationsStartEnd();
        if(spacePressed && currentUpdateState == UpdateState::START)
            GoToCountdownAnimation();
        else if(spacePressed)
            GoToStateEndrace();
    }
}

bool StateInGameSingle::UpdateAnimationStart() {
    bool animationFinished = StateInGame::UpdateAnimationStart();
    if(animationFinished)
        GoToCountdownAnimation();
    return animationFinished;
}

void StateInGameSingle::UpdateAnimationCountdown() {
    StateInGame::UpdateAnimationCountdown();
}

void StateInGameSingle::UpdateAnimationEnd() {
    StateInGame::UpdateAnimationEnd();
}

void StateInGameSingle::Update() {
    switch (currentUpdateState) {
        case UpdateState::START:
            UpdateAnimationStart();
            break;
        case UpdateState::COUNTDOWN:
            UpdateAnimationCountdown();
            break;
        case UpdateState::END:
            UpdateAnimationEnd();
            break;
        case UpdateState::GAME:
            UpdateGame();
            break;

        default:
            //cout << "currentUpdateState inválido" << endl;
            break;
    }
}

void StateInGameSingle::UpdateGame() {
    //std::cout.precision(10);
    //timeStart =  std::chrono::system_clock::now();


    // si estamos yendo a pausa, paramos los temporizadores
    if (goingToPause) {
        timeStartPause = Utils::getMillisSinceEpoch();
        goingToPause = false;
        comingBackFromPause = true;
        //cout << "Vamos a ir a pausa, teóricamente paramos los temporizadores" << endl;

        // si volvemos de pausa, reiniciamos los temporizadores
    } else if (comingBackFromPause) {
        //cout << "Volvemos de pausa, reiniciamos los temporizadores" << endl;
        manGameRules->RestartAllTimers(manCars->GetEntities(), timeStartPause);
        comingBackFromPause = false;
    }

    //timeStartSeccion = std::chrono::system_clock::now();


    StateInGame::UpdateGame();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    auto end = std::chrono::system_clock::now();
//    double elapsed_millisecons = std::chrono::duration_cast<std::chrono::nanoseconds>
//                             (end-timeStartSeccion).count();
//    cout << "TIEMO ACTUAL ULDATE  (COSAS DEL STATE IN GAME (mecanicas, colisiones, etc)):  " << elapsed_millisecons/1000000 << endl;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    timeStartSeccion = std::chrono::system_clock::now();

    manAI->Update();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    end = std::chrono::system_clock::now();
//    elapsed_millisecons = std::chrono::duration_cast<std::chrono::nanoseconds>
//                             (end-timeStartSeccion).count();
//    cout << "TIEMO ACTUAL ULDATE  (LA IA):  " << elapsed_millisecons/1000000 << endl;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (auto actualAI : manCars->GetEntities()) {  // CUIDADO!!! -> el static cast que solo se use en el single player, si no peta
        if (static_cast<Car *>(actualAI.get())->GetTypeCar() == TypeCar::CarAI) {
            bool gameFinished = manCars->UpdateCarAI(static_cast<CarAI *>(actualAI.get()), manTotems.get());
            if(gameFinished) 
                GoToEndAnimation();
        }
    }
    for (auto actualAI : manCars->GetEntities()) {  // CUIDADO!!! -> el static cast que solo se use en el single player, si no peta
        if (static_cast<Car *>(actualAI.get())->GetTypeCar() == TypeCar::CarAI) {
            physicsEngine->UpdateTransformable(actualAI.get());
        }
    }

    //ACTUALIZAR ESCUDOS
    physicsEngine->UpdateShields(manCars->GetEntities(), manShield->GetEntities());

//    end = std::chrono::system_clock::now();
//    elapsed_millisecons = std::chrono::duration_cast<std::chrono::nanoseconds>
//                             (end-timeStart).count();
//    cout << "TIEMO ACTUAL ULDATE:  " << elapsed_millisecons/1000000 << endl;
//    if(accumulatedTimeUPDATE < elapsed_millisecons){
//        accumulatedTimeUPDATE = elapsed_millisecons;
//    }
//    cout << "TIEMO MAXIMO UPDATE:  " << accumulatedTimeUPDATE/1000000 << endl;

}

void StateInGameSingle::Render() {
    //std::cout.precision(10);
    timeStart =  std::chrono::system_clock::now();
    
    auto carPrincial = manCars->GetCar().get();
    bool isColliding = collisions->Intersects(manCars.get()->GetCar().get(), carPrincial);
    renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), isColliding);

    for (auto cars : manCars->GetEntities()) {
        renderEngine->FacadeDrawBoundingBox(cars.get(), false);
    }
    //renderEngine->FacadeDrawBoundingBox(carPrincial, isColliding);
    StateInGame::Render();

    auto end = std::chrono::system_clock::now();
    int elapsed_millisecons = std::chrono::duration_cast<std::chrono::nanoseconds>
                             (end-timeStart).count();
    //cout << "TIEMO ACTUAL RENDER:  " << elapsed_millisecons/1000000 << endl;
    if(accumulatedTimeRENDER < elapsed_millisecons && elapsed_millisecons/1000000 < 30.0){
        accumulatedTimeRENDER = elapsed_millisecons;
    }
    //cout << "TIEMO MAXIMO RENDER:  " << accumulatedTimeRENDER/1000000 << endl;
}

void StateInGameSingle::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    StateInGame::InitializeCLPhysics(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
}

void StateInGameSingle::InitializeManagers() {
    StateInGame::InitializeManagers();
}

void StateInGameSingle::InitializeSystems(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    StateInGame::InitializeSystems(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
}

void StateInGameSingle::InitializeFacades() {
    StateInGame::InitializeFacades();
}

void StateInGameSingle::AddElementsToRender() {
    StateInGame::AddElementsToRender();
}

void StateInGameSingle::createSystemAI() {
    // creamos ManAI
    manAI = make_unique<ManAI>();

    // iniciamos los sistemas
    InitVision();
    InitBtMoveTo();
    InitPathPlanning();
    InitBtLoDMove();
    InitBtPowerUp();
    InitBtDecisionMove();

    //creamos comportamientos IA
    uint32_t i = 0;
    for(auto actualAI : manCars->GetEntities()){
        if (static_cast<Car*>(actualAI.get())->GetTypeCar() == TypeCar::CarAI){
            manAI->addBehavior(static_cast<CarAI*>(actualAI.get()), systemVisionAI.get(),       systemVisionAI->getFrecuency(),         i, systemVisionAI.get()->getMaxProcessTime() );
            manAI->addBehavior(static_cast<CarAI*>(actualAI.get()), systemBtDecisionMove.get(), systemBtDecisionMove->getFrecuency(),   i, systemBtDecisionMove.get()->getMaxProcessTime() );
            //manAI->addBehavior(static_cast<CarAI*>(actualAI.get()), systemBtMoveTo.get(),     systemBtMoveTo->getFrecuency(),         i, systemBtMoveTo.get()->getMaxProcessTime() );
            manAI->addBehavior(static_cast<CarAI*>(actualAI.get()), systemPathPlanning.get(),   systemPathPlanning->getFrecuency(),     i, systemPathPlanning.get()->getMaxProcessTime() );
            manAI->addBehavior(static_cast<CarAI*>(actualAI.get()), systemBtLoDMove.get(),      systemBtLoDMove->getFrecuency(),        i, systemBtLoDMove.get()->getMaxProcessTime() );
            manAI->addBehavior(static_cast<CarAI*>(actualAI.get()), systemBtPowerUp.get(),      systemBtPowerUp->getFrecuency(),        i, systemBtPowerUp.get()->getMaxProcessTime() );
            
            i++;
        }
    }
}

void StateInGameSingle::InitBtPowerUp() {
    systemBtPowerUp = make_unique<SystemBtPowerUp>();

    systemBtPowerUp->AddManager(*manCars.get());
    systemBtPowerUp->AddManager(*manPowerUps.get());
    systemBtPowerUp->AddManager(*manBoxPowerUps.get());
    systemBtPowerUp->AddManager(*manTotems.get());
    systemBtPowerUp->AddManager(*manWayPoint.get());
    systemBtPowerUp->AddManager(*manNavMesh.get());
    systemBtPowerUp->AddManager(*manBoundingWall.get());
    systemBtPowerUp->AddManager(*manBoundingOBB.get());
    // Precalculado
    systemBtPowerUp->setMaxProcessTime(0.00025);
}
void StateInGameSingle::InitBtMoveTo() {
    systemBtMoveTo = make_unique<SystemBtMoveTo>();

    systemBtMoveTo->AddManager(*manCars.get());
    systemBtMoveTo->AddManager(*manPowerUps.get());
    systemBtMoveTo->AddManager(*manBoxPowerUps.get());
    systemBtMoveTo->AddManager(*manTotems.get());
    systemBtMoveTo->AddManager(*manWayPoint.get());
    systemBtMoveTo->AddManager(*manNavMesh.get());
    systemBtMoveTo->AddManager(*manBoundingWall.get());
    systemBtMoveTo->AddManager(*manBoundingOBB.get());

    systemBtMoveTo->setMaxProcessTime(0.00035);
}
void StateInGameSingle::InitBtLoDMove() {
    systemBtLoDMove = make_unique<SystemBtLoDMove>();

    systemBtLoDMove->AddManager(*manCars.get());
    systemBtLoDMove->AddManager(*manPowerUps.get());
    systemBtLoDMove->AddManager(*manBoxPowerUps.get());
    systemBtLoDMove->AddManager(*manTotems.get());
    systemBtLoDMove->AddManager(*manWayPoint.get());
    systemBtLoDMove->AddManager(*manNavMesh.get());
    systemBtLoDMove->AddManager(*manBoundingWall.get());
    systemBtLoDMove->AddManager(*manBoundingOBB.get());
    
    systemBtLoDMove->AddCLPhysicsSB(clPhysics.get());
    systemBtLoDMove->InitFuzzyLogic(*manCars.get());

    systemBtLoDMove->setMaxProcessTime(0.00053);
}
void StateInGameSingle::InitPathPlanning() {
    systemPathPlanning = make_unique<SystemPathPlanning>();

    systemPathPlanning->AddManager(*manCars.get());
    systemPathPlanning->AddManager(*manPowerUps.get());
    systemPathPlanning->AddManager(*manBoxPowerUps.get());
    systemPathPlanning->AddManager(*manTotems.get());
    systemPathPlanning->AddManager(*manWayPoint.get());
    systemPathPlanning->AddManager(*manNavMesh.get());
    systemPathPlanning->AddManager(*manBoundingWall.get());
    systemPathPlanning->AddManager(*manBoundingOBB.get());

    systemPathPlanning->setMaxProcessTime(0.00025);
}

void StateInGameSingle::InitVision(){
    systemVisionAI = make_unique<SystemVisionAI>(clPhysics.get());

    systemVisionAI->AddManager(*manCars.get());
    systemVisionAI->AddManager(*manPowerUps.get());
    systemVisionAI->AddManager(*manBoxPowerUps.get());
    systemVisionAI->AddManager(*manTotems.get());
    systemVisionAI->AddManager(*manWayPoint.get());
    systemVisionAI->AddManager(*manNavMesh.get());
    systemVisionAI->AddManager(*manBoundingWall.get());
    systemVisionAI->AddManager(*manBoundingOBB.get());
    systemVisionAI->AddManager(*manBoundingGround.get());

    systemVisionAI->setMaxProcessTime(0.00025);
}

void StateInGameSingle::InitBtDecisionMove(){
    systemBtDecisionMove = make_unique<SystemBtDecisionMove>();

    systemBtDecisionMove->AddManager(*manCars.get());
    systemBtDecisionMove->AddManager(*manPowerUps.get());
    systemBtDecisionMove->AddManager(*manBoxPowerUps.get());
    systemBtDecisionMove->AddManager(*manTotems.get());
    systemBtDecisionMove->AddManager(*manWayPoint.get());
    systemBtDecisionMove->AddManager(*manNavMesh.get());
    systemBtDecisionMove->AddManager(*manBoundingWall.get());
    systemBtDecisionMove->AddManager(*manBoundingOBB.get());

    systemBtDecisionMove->setMaxProcessTime(0.00050);
}



void StateInGameSingle::InitCarAIS(ManCar &manCars, ManWayPoint &manWayPoint) {
    auto iaPjs = GameValues::GetInstance()->GetIACharacters();
    auto numPlayers = GameValues::GetInstance()->GetNumPlayers();
    int iaDifficult = GameValues::GetInstance()->GetDifficultAI();
    float timeTotem = GameValues::GetInstance()->GetTimeTotem();

    //auto posCar1 = glm::vec3(290.0f, 15.0f, -300.0f);
    //auto posCar2 = glm::vec3(-202.0f, 15.0f, -145.0f);
    //auto posCar3 = glm::vec3(209.0f, 15.0f, -145.0f);

    //Para asegurarse por si petara, que no debe
    if (!iaPjs.empty()) {
        if (iaPjs.size() < 5) {
            //cout << "++++++++++ El vector de IA en GameValues no tiene el tamanyo que debe tener. Si peta despues de esto, buscame.";
        }

        //Cambiar
        for (uint8_t i = 0; i < (numPlayers - 1); ++i) {
            manCars.CreateCarAI(iaPjs[i], iaDifficult, timeTotem, manCars.GetPosSpawn());
        }
        //manCars.CreateCarAI(iaPjs[0], manCars.GetPosSpawn());
        //manCars.CreateCarAI(iaPjs[1], manCars.GetPosSpawn());

    } else {
        //cout << "++++++++++ Algo no va bien asique ahora todos son pinguinos.";
        for (uint8_t i = 0; i < (numPlayers - 1); ++i) {
            manCars.CreateCarAI(0, iaDifficult, timeTotem, manCars.GetPosSpawn());
        }
    }

    //Añadimos las nameplates
    for(auto car : manCars.GetEntities()){
        if(manCars.GetCar().get() != car.get()){
            manNamePlates->CreateNamePlate(car.get());
        }
    }

    //auto mainCarId = static_cast<CId *>(manCars.GetCar()->GetComponent(CompType::IdComp).get());
    //int i = -1;
    //TODO: Cambiar de sitio
    for (auto e : manCars.GetEntities()) {
        //i++;
        //if (i != 0) {
        auto idComp = static_cast<CId *>(e->GetComponent(CompType::IdComp).get());
        auto posComp = static_cast<CTransformable *>(e->GetComponent(CompType::TransformableComp).get());
        string nameEvent = "Coche/motor";
        SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(idComp->id, posComp->position, nameEvent, 1, 0);
        nameEvent = "PowerUp/escudo";
        SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundDinamic3D(idComp->id, posComp->position, nameEvent, 0, 0);
        nameEvent = "PowerUp/escudo_roto";
        SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(idComp->id, posComp->position, nameEvent, 0);
        nameEvent = "PowerUp/choque_powerup";
        SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(idComp->id, posComp->position, nameEvent, 0);
        nameEvent = "Coche/choque";
        SoundFacadeManager::GetInstance()->GetSoundFacade()->CreateSoundEstatic3D(idComp->id, posComp->position, nameEvent, 0);

        //Lo meto aqui aunque pone cambiar de sitio porque no queda otra
        manShield->CreateShield(idComp->id,glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(3.0f));
    }
}