#include "StateInGame.h"
#include <chrono>
#include <iostream>

#include <CLPhysics/CLPhysics.h>
#include <Components/CMesh.h>
#include <Components/CTexture.h>
#include "../Components/CBoundingSphere.h"
#include "../Components/CShader.h"
#include "../Components/CBoundingCilindre.h"
#include "../Components/CTotem.h"
#include "../Constants.h"
#include <GameValues.h>

using namespace std;
using namespace chrono;

StateInGame::StateInGame() {

    std::cout << "> INGAME constructor" << std::endl;

    // aunque physics es un sistema, no se llama desde InitializeSystems
    // porque tiene que estar inicializado antes de llamar a InitializeManagers
    cout << "------------------------------------------------------------------" << endl;
    cout << "delta vale" << Constants::DELTA_TIME << endl;
    cout << "------------------------------------------------------------------" << endl;
    physics = make_unique<Physics>(Constants::DELTA_TIME);

    cam = make_shared<Camera>(glm::vec3(100.0f, 0.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    ground = make_shared<GameObject>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "", "training_ground.obj");

}

StateInGame::~StateInGame() {
    // destructor
}

/**
 * IMPORTANTE LEER ESTO!!
 * Debido a las características de c++, estos métodos no se pueden llamar desde el constructor
 *  porque son virtuales y en la ejecución del constructor padre (este), el hijo todavía
 *  no está construido. Eso significa que cuando estos métodos se llaman desde el constructor,
 *  no se están llamando a los métodos de los hijos, sino al nuestro (padre).
 *  Como estos métodos son virtuales, nuestros hijos los están sobreescribiendo
 *  y por tanto, necesitamos que se ejecuten con sus cambios de cada uno de sus hijos.
 *  EN RESUMEN: NO se deben llamar a métodos virtuales desde el constructor de la clase que los declara virtuales.
 *  Y ES OBLIGATORIO llamar a este método desde el constructor de los hijos
 */
void StateInGame::InitVirtualMethods() {
    auto gameTime = GameValues::GetInstance()->GetGameTime();
    InitializeManagers(physics.get(), cam.get(), gameTime);
    InitializeSystems(*manCars.get(), *manBoundingWall.get(), *manBoundingOBB.get(), *manBoundingGround.get(), *manPowerUps.get(), *manNavMesh.get(), *manBoxPowerUps.get(), *manTotems.get());
    InitializeFacades();

    //CAMBIARCosasDeTotem(*manTotems.get());
    //CAMBIARCosasDeBoxPU(*manWayPoint.get(), *manBoxPowerUps.get());
    //CAMBIARCosasNavMesh(*manNavMesh.get());

    // esta llamada lo ideal es que sea la última porque hace uso de todo
    // lo anterior y debe de estar todo inicializado
    AddElementsToRender();
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
    // Añadimos cosas a la fachada de render
    renderEngine->FacadeAddPlates(manNamePlates.get());

    // Entidades iniciales
    renderEngine->FacadeAddObjectCar(manCars.get()->GetCar().get());  //Anyadimos el coche
    for (auto cars : manCars->GetEntities()){  // Anyadimos los coches
        if(manCars.get()->GetCar().get() != cars.get())
            renderEngine->FacadeAddObject(cars.get());
    }


    renderEngine->FacadeAddObject(ground.get());  //Anyadimos el suelo

    //Añadimos todos los power ups
    for (auto bpu : manBoxPowerUps->GetEntities())
        renderEngine->FacadeAddObject(bpu.get());

    renderEngine->FacadeAddCamera(cam.get());

    renderEngine->FacadeAddObjectTotem(manTotems->GetEntities()[0].get());
    
    //Añadimos las luces
    for(auto light : manLight->GetEntities()){
        renderEngine->FacadeAddObject(light.get());
    }

    renderEngine->FacadeAddSkybox("media/skybox/right.jpg",
        "media/skybox/left.jpg",
        "media/skybox/top.jpg",
        "media/skybox/bottom.jpg",
        "media/skybox/front.jpg",
        "media/skybox/back.jpg");
    
    if(manLight->GetEntities().size()>0){
        auto lightWithShadow = manLight->GetEntities()[0];
        auto cId = static_cast<CId*>(lightWithShadow->GetComponent(CompType::IdComp).get());
        renderEngine->FacadeAddShadowMapping(cId->id);
    }

    for(auto particleSystem : manParticleSystem->GetEntities()){
        renderEngine->FacadeAddObject(particleSystem.get());
    }
}

void StateInGame::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
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

void StateInGame::InitializeSystems(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    InitializeCLPhysics(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
    // incializa el system physics PU, no hace falta más código para esto
    phisicsPowerUp = make_shared<PhysicsPowerUp>();  // Creamos sistemas
    collisions = make_shared<Collisions>();
    sysBoxPowerUp = make_shared<SystemBoxPowerUp>();
    sysLoD = make_unique<SystemLoD>();
    sysRanking = make_unique<SystemRanking>();
}

void StateInGame::InitializeManagers(Physics *physics, Camera *cam, const uint32_t timeGame) {
    // inicializa el man PU, no hace falta más código para esto
    manCars             = make_shared<ManCar>(physics, cam);
    manWayPoint         = make_shared<ManWayPoint>();  //Se crean todos los waypoints y edges
    manPowerUps         = make_shared<ManPowerUp>(manCars);
    manBoxPowerUps      = make_shared<ManBoxPowerUp>();
    manBoundingWall     = make_shared<ManBoundingWall>();
    manBoundingOBB      = make_shared<ManBoundingOBB>();
    manBoundingGround   = make_shared<ManBoundingGround>();
    manNavMesh          = make_shared<ManNavMesh>();
    manTotems           = make_shared<ManTotem>(manNavMesh.get(), 45);
    manNamePlates       = make_shared<ManNamePlate>(manCars.get());
    manLight            = make_shared<ManLight>();
    manGameRules        = make_unique<ManGameRules>(timeGame);
    manParticleSystem   = make_unique<ManParticleSystem>();

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
    for(auto boxPowerUp : manBoxPowerUps->GetEntities()){
        auto cId = static_cast<CId*>(boxPowerUp->GetComponent(CompType::IdComp).get());
        manParticleSystem->CreateParticleSystem(cId->id,glm::vec3(0.0f,0.0f,0.0f),30,glm::vec3(200.0f,400.0f,200.0f),puParticles,5,5,100,30,150,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f),0, 0x1 | 0x4 ,false,false);
    }

    // Añadimos sistema de particulas a los (el) totem
    vector<string> totemParticles;
    totemParticles.push_back("media/particle_test.png");
    for(auto totem : manTotems->GetEntities()){
        auto cId = static_cast<CId*>(totem->GetComponent(CompType::IdComp).get());
        manParticleSystem->CreateParticleSystem(cId->id,glm::vec3(0.0f,0.0f,0.0f),100,glm::vec3(0.0f,50.0f,0.0f),totemParticles,5,15,100,2,5000,glm::vec3(30.0f,0.0f,30.0f),glm::vec3(0.0f,0.0f,0.0f),0, 0x4 ,true,true);
        
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
    if(manCars) {
        auto pj = GameValues::GetInstance()->GetCharacter();
        auto timeTotem = GameValues::GetInstance()->GetTimeTotem();
        manCars->CreateMainCar(pj, timeTotem);
        /*auto cCar = static_cast<CCar*>(manCars->GetCar()->GetComponent(CompType::CarComp).get());
        if (cCar){
            cout << "PESO: " << cCar->weight << " - VELMAX: " << cCar->maxSpeed << " - ACELETARION: " << cCar->acceleration << "\n";
        }*/
    }
}

///////////////////////

///////////////////////

void StateInGame::Update() {


    EventManager &em = EventManager::GetInstance();
    em.Update();

    manNavMesh->Update(*(manCars.get()));

    // ACTUALIZACION DE LOS MANAGERS DE LOS COCHES
    manCars->UpdateCarPlayer(*(manTotems.get()));
    // ACTUALIZACION DE LAS FISICAS DE LOS COCHES
    physics->update(manCars->GetCar().get(), cam.get());

    sysBoxPowerUp->update(manBoxPowerUps.get());
    for(auto& actualPowerUp : manPowerUps->GetEntities()){  // actualizamos las fisicas de los powerUps
        phisicsPowerUp->update(actualPowerUp.get());
    }

    clPhysics->Update(0.1666f);
    clPhysics->IntersectsCarsPowerUps( *manCars.get(), *manPowerUps.get(), manNavMesh.get());
    clPhysics->IntersectCarsBoxPowerUp(*manCars.get(), *manBoxPowerUps.get());
    clPhysics->IntersectCarsTotem(*manCars.get(), *manTotems.get());
    clPhysics->IntersectPowerUpWalls(*manPowerUps.get(), *manBoundingWall.get(), *manBoundingOBB.get());

    // Actualizaciones en Irrlich
    renderEngine->UpdateCamera(cam.get(), manCars.get());
    physicsEngine->UpdateCar(manCars.get()->GetCar().get(), cam.get());

    for (auto actualPowerUp : manPowerUps->GetEntities())  // actualizamos los powerUp en irrlich
        physicsEngine->UpdatePowerUps(actualPowerUp.get());

    renderEngine->FacadeUpdatePlates(manNamePlates.get());
    physicsEngine->UpdateTransformable(manTotems->GetEntities()[0].get());

    //Updates de los eventos de sonido
    soundEngine->UpdateCars(manCars->GetEntities());
    soundEngine->UpdatePowerUps(manPowerUps->GetEntities());
    soundEngine->UpdateTotem(manTotems->GetEntities());       
    soundEngine->UpdateListener(manCars->GetCar());

    // al final de la ejecucion eliminamos todos los powerUps que se deben eliminar
    manPowerUps->Update();

    sysLoD->UpdateMeshes(manCars->GetEntities(), cam.get());
    sysLoD->UpdateMeshes(manPowerUps->GetEntities(), cam.get());
    sysLoD->UpdateMeshes(manTotems->GetEntities(), cam.get());
    sysLoD->UpdateAnimations(manBoxPowerUps->GetEntities(), cam.get());

    renderEngine->FacadeUpdateMeshesLoD(manCars->GetEntities());
    renderEngine->FacadeUpdateMeshesLoD(manPowerUps->GetEntities());
    renderEngine->FacadeUpdateMeshesLoD(manTotems->GetEntities());
    renderEngine->FacadeUpdateAnimationsLoD(manBoxPowerUps->GetEntities());

    renderEngine->FacadeAnimate(manBoxPowerUps->GetEntities());

    sysRanking->Update(manCars.get());
    manGameRules->Update();

    //if(octreeI == 0){
    //    octreeI++;
    //}
    //octreeScene = make_unique<Octree>(glm::vec3(0.0, 500.0, 0.0), 700.0, managersEntities);
    //octreeScene->UpdateVisibleObjects(renderEngine);
}

void StateInGame::Render() {

    renderEngine->FacadeBeginScene();
    // renderEngine->FacadeDraw();  //Para dibujar primitivas debe ir entre el drawAll y el endScene
    renderEngine->FacadeDrawAll();
    
    //if(octreeI>0)
    //    octreeScene->Draw(renderEngine);
    renderEngine->FacadeDrawHUD(manCars->GetCar().get(), manCars.get(), manGameRules->GetGlobalClock().get());
    renderEngine->FacadeDrawGraphEdges(manWayPoint.get());
    // renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), isColliding);

    for (auto& actualPowerUp : manPowerUps->GetEntities()){
        renderEngine->FacadeDrawBoundingBox(actualPowerUp.get(), false);
    }
    for (auto& wall : manBoundingWall->GetEntities()) {
        renderEngine->FacadeDrawBoundingPlane(wall.get());
    }
    for (auto& ground : manBoundingGround->GetEntities()) {
        renderEngine->FacadeDrawBoundingGround(ground.get());
    }
    for (auto& obb : manBoundingOBB->GetEntities()) {
        renderEngine->FacadeDrawBoundingOBB(obb.get());
    }


    renderEngine->FacadeDrawAIDebug(manCars.get(),manNavMesh.get(), manWayPoint.get());
    renderEngine->FacadeEndScene();
}