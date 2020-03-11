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

using namespace std;
using namespace chrono;

StateInGame::StateInGame() {
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
    InitializeManagers(physics.get(), cam.get());
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
}

/*
void StateInGame::CAMBIARCosasDeTotem(ManTotem &manTotems) {
    // --------------------------------------------------------------------------------------------------------------------------------------------
    totemOnCar = make_shared<Entity>();
    glm::vec3 postoTemOnCar = glm::vec3(40.0f, -100.0f, 30.0f);
    glm::vec3 rotTotemOnCar = glm::vec3(0.0f, 90.0f, 0.0f);
    glm::vec3 scaleTotemOnCar = glm::vec3(0.5f, 0.5f, 0.5f);
    shared_ptr<CId> cIdTotemOnCar = make_shared<CId>();
    shared_ptr<CType> cTypeTotemOnCar = make_shared<CType>(ModelType::Cube);
    shared_ptr<CTransformable> cTransformableTotemOnCar = make_shared<CTransformable>(postoTemOnCar, rotTotemOnCar, scaleTotemOnCar);
    totemOnCar->AddComponent(cIdTotemOnCar);
    totemOnCar->AddComponent(cTypeTotemOnCar);
    totemOnCar->AddComponent(cTransformableTotemOnCar);
    totemOnCar->AddComponent(make_shared<CTexture>("totem.jpg"));
    totemOnCar->AddComponent(make_shared<CMesh>("kart_ia.obj"));
    totemOnCar->AddComponent(make_shared<CShader>("CLEngine/src/Shaders/vertex.glsl","CLEngine/src/Shaders/fragment.glsl"));
    // ------------------------------------------------------------------------------------------------------------------------------------------------
}
*/
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
    // este último probablemente haya que cambiarlo ¿?
    //renderEngine->FacadeAddObject(totemOnCar.get());
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
}

void StateInGame::InitializeManagers(Physics *physics, Camera *cam) {
    // inicializa el man PU, no hace falta más código para esto
    manCars = make_shared<ManCar>(physics, cam);
    manWayPoint = make_shared<ManWayPoint>();  //Se crean todos los waypoints y edges
    manPowerUps = make_shared<ManPowerUp>(manCars);
    manBoxPowerUps = make_shared<ManBoxPowerUp>();
    manBoundingWall = make_shared<ManBoundingWall>();
    manBoundingOBB = make_shared<ManBoundingOBB>();
    manBoundingGround = make_shared<ManBoundingGround>();
    manNavMesh = make_shared<ManNavMesh>();
    manTotems = make_shared<ManTotem>(manNavMesh.get());
    manNamePlates = make_shared<ManNamePlate>(manCars.get());
    manGameRules = make_unique<ManGameRules>();
}

//Carga los bancos de sonido InGame.
void StateInGame::InitState() {
    cout << "~~~ ENTRO A INGAME" << endl;

    //Si la direccion de soundEngine!=0 es que viene del PAUSE, por lo que no deberia hacerlo.
    if (!soundEngine) {
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
        //cout << "~~~ SoundEngine en INGAME es -> " << soundEngine << endl;
        if (soundEngine) {
            soundEngine->SetState(4);
        }
    } else {
        soundEngine->ResumeAllEvent();
    }
}

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
}

void StateInGame::Render() {

    renderEngine->FacadeBeginScene();
    // renderEngine->FacadeDraw();  //Para dibujar primitivas debe ir entre el drawAll y el endScene
    renderEngine->FacadeDrawAll();
    renderEngine->FacadeDrawHUD(manCars->GetCar().get(), manCars.get());
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