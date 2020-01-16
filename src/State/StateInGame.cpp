#include "StateInGame.h"
#include <limits.h>
#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>

#include "../CLPhysics/CLPhysics.h"
#include "../Components/CMesh.h"
#include "../Components/CNamePlate.h"
#include "../Components/CPath.h"
#include "../Components/CTexture.h"
#include "../Components/CTotem.h"
#include "../Components/CTransformable.h"
#include "../Components/CWayPointEdges.h"
#include "../Managers/Manager.h"

typedef std::chrono::high_resolution_clock Clock;

using namespace std;
using namespace chrono;

StateInGame::StateInGame() {
    // aunque physics es un sistema, no se llama desde InitializeSystems
    // porque tiene que estar inicializado antes de llamar a InitializeManagers
    physics = make_unique<Physics>(deltaTime);

    cam = make_shared<Camera>(glm::vec3(100.0f, 600.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    ground = make_shared<GameObject>(glm::vec3(10.0f, -0.5f, 150.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.5f, 1.5f, 1.5f), "", "training_ground.obj");
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
    InitializeSystems(*manCars.get(), *manBoundingWall.get());
    InitializeFacades();

    CAMBIARCosasDeTotem(*manTotems.get());
    CAMBIARCosasDeBoxPU(*manWayPoint.get(), *manBoxPowerUps.get());
    CAMBIARCosasNavMesh(*manNavMesh.get());

    // esta llamada lo ideal es que sea la última porque hace uso de todo
    // lo anterior y debe de estar todo inicializado
    AddElementsToRender();
}

void StateInGame::CAMBIARCosasNavMesh(ManNavMesh &manNavMesh){
    // CREAMOS DE PRUEBA UN NAVMESH
    vector<int> waypoints0{0,1,2,3,4,12};
    vector<int> waypoints1{7,8,9,10,11,13};
    vector<int> waypoints2{3,5,7};  // el 5 debe ser referencia
    vector<int> waypoints3{4,6,8};  // el 6 debe ser referencia
    manNavMesh.CreateNavMesh(glm::vec3(0.0f,0.0f,-200.0f),glm::vec3(0.0f,0.0f,0.0f),1000,50,500,waypoints0);  //0
    manNavMesh.CreateNavMesh(glm::vec3(0.0f,0.0f,500.0f),glm::vec3(0.0f,0.0f,0.0f),1000,50,500,waypoints1);   //1
    manNavMesh.CreateNavMesh(glm::vec3(-300.0f,0.0f,150.0f),glm::vec3(0.0f,0.0f,0.0f),150,50,200,waypoints2); //2
    manNavMesh.CreateNavMesh(glm::vec3(300.0f,0.0f,150.0f),glm::vec3(0.0f,0.0f,0.0f),150,50,200,waypoints3);  //3


    auto cTransformableCar = static_cast<CTransformable*>(manCars.get()->GetCar().get()->GetComponent(CompType::TransformableComp).get());     
    for(auto navmesh : manNavMesh.GetEntities()){
        auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (cTransformableCar->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
            (cTransformableCar->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
            ( (cTransformableCar->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
            (cTransformableCar->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(manCars.get()->GetCar().get()->GetComponent(CompType::CurrentNavMeshComp).get());
                auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                //std::cout << " El cochecito lereee pertenece al naveMesh: " << cNavMesh->id << std::endl;
            }       
    }


    auto cTransformableTotem = static_cast<CTransformable*>(manTotems->GetEntities()[0].get()->GetComponent(CompType::TransformableComp).get());     
    for(auto navmesh : manNavMesh.GetEntities()){
        auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (cTransformableTotem->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
            (cTransformableTotem->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
            ( (cTransformableTotem->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
            (cTransformableTotem->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(manTotems->GetEntities()[0].get()->GetComponent(CompType::CurrentNavMeshComp).get());
                auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                //std::cout << " El cochecito lereee pertenece al naveMesh: " << cNavMesh->id << std::endl;
            }       
    }

}

void StateInGame::CAMBIARCosasDeBoxPU(ManWayPoint &manWayPoint, ManBoxPowerUp &manBoxPowerUps) {
    for (shared_ptr<WayPoint> way : manWayPoint.GetEntities()) {
        auto components = way->GetComponents();
        auto mapWaypoint = components.find(CompType::WayPointComp);
        auto cWayPoint = static_cast<CWayPoint *>(mapWaypoint->second.get());
        // solo debemos crear las Box si el type del waypoint es "1"
        if (cWayPoint->type == 1) {
            manBoxPowerUps.CreateBoxPowerUp(glm::vec3(cWayPoint->position));
        }
    }
}

void StateInGame::InitializeFacades() {
    // Inicializamos las facadas
    inputEngine = InputFacadeManager::GetInstance()->GetInputFacade();
    physicsEngine = PhysicsFacadeManager::GetInstance()->GetPhysicsFacade();
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
}

void StateInGame::CAMBIARCosasDeTotem(ManTotem &manTotems) {
    // CREAMOS EL TOTEM
    manTotems.CreateTotem(glm::vec3(-100.0, 20.0, -100.0));
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
    totemOnCar->AddComponent(make_shared<CMesh>("media/ninja.b3d"));
    // ------------------------------------------------------------------------------------------------------------------------------------------------
}

void StateInGame::AddElementsToRender() {
    // Añadimos cosas a la fachada de render
    renderEngine->FacadeAddPlates(manNamePlates.get());

    // Entidades iniciales
    renderEngine->FacadeAddObjectCar(manCars.get()->GetCar().get());  //Anyadimos el coche

    renderEngine->FacadeAddObject(ground.get());  //Anyadimos el suelo

    //Añadimos todos los power ups
    for (shared_ptr<Entity> bpu : manBoxPowerUps->GetEntities())
        renderEngine->FacadeAddObject(bpu.get());

    renderEngine->FacadeAddCamera(cam.get());

    renderEngine->FacadeAddObjectTotem(manTotems->GetEntities()[0].get());
    // este último probablemente haya que cambiarlo ¿?
    renderEngine->FacadeAddObject(totemOnCar.get());
}

void StateInGame::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manBoundingWall) {
    // NO ALTERAR EL ORDEN DEL ADD, QUE USO EL ORDEN PARA DISTINGUIR ENTRE MANAGERS!!!
    clPhysics = make_unique<CLPhysics>();
    clPhysics->AddManager(manCars);
    clPhysics->AddManager(manBoundingWall);
}

void StateInGame::InitializeSystems(ManCar &manCars, ManBoundingWall &manBoundingWall) {
    InitializeCLPhysics(manCars, manBoundingWall);
    // incializa el system physics PU, no hace falta más código para esto
    phisicsPowerUp = make_shared<PhysicsPowerUp>();  // Creamos sistemas
    collisions = make_shared<Collisions>();
    sysBoxPowerUp = make_shared<SystemBoxPowerUp>();
}

void StateInGame::InitializeManagers(Physics *physics, Camera *cam) {
    // inicializa el man PU, no hace falta más código para esto
    manCars = make_shared<ManCar>(physics, cam);
    manWayPoint = make_shared<ManWayPoint>();  //Se crean todos los waypoints y edges
    manPowerUps = make_shared<ManPowerUp>();
    manBoxPowerUps = make_shared<ManBoxPowerUp>();
    manBoundingWall = make_shared<ManBoundingWall>();
    manTotems = make_shared<ManTotem>();
    manNavMesh = make_shared<ManNavMesh>();
}

//Carga los bancos de sonido InGame.
void StateInGame::InitState() {
    cout << "~~~ ENTRO A INGAME" << endl;

    //Si la direccion de soundEngine!=0 es que viene del PAUSE, por lo que no deberia hacerlo.
    if (!soundEngine) {
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
        cout << "~~~ SoundEngine en INGAME es -> " << soundEngine << endl;
        if (soundEngine) {
            soundEngine->SetState(4);
            EventManager::GetInstance().AddEventMulti(Event{EventType::START_GAME});
        }
    } else {
        soundEngine->ResumeAllEvent();
    }
}

void StateInGame::Input() {
    renderEngine->FacadeCheckInput();
    inputEngine->CheckInput();
}

void StateInGame::Update() {
    EventManager &em = EventManager::GetInstance();
    em.Update();

    //ACTUALIZAMOS MANAGER NAVMESH CAR PLAYER
    manNavMesh->UpdateNavMeshPlayer(manCars.get()->GetCar().get());
    // ACTUALIZACION DE LOS MANAGERS DE LOS COCHES
    manCars->UpdateCar();

    // ACTUALIZACION DE LAS FISICAS DE LOS COCHES
    physics->update(manCars->GetCar().get(), cam.get());

    clPhysics->Update(0.1666f);
    sysBoxPowerUp->update(manBoxPowerUps.get());
    phisicsPowerUp->update(manPowerUps->GetEntities());

    // COLISIONES entre BoxPowerUp y player
    collisions->IntersectPlayerBoxPowerUp(manCars.get()->GetCar().get(), manBoxPowerUps.get());
    // COLISIONES entre powerUp y player
    collisions->IntersectPlayerPowerUps(manCars.get()->GetCar().get(), manPowerUps.get(), manNavMesh.get());
    // COLISIONES entre el Player y el Totem
    collisions->IntersectPlayerTotem(manCars.get()->GetCar().get(), manTotems.get());

    // Actualizaciones en Irrlich
    renderEngine->UpdateCamera(cam.get(), manCars.get());
    physicsEngine->UpdateCar(manCars.get()->GetCar().get(), cam.get());

    for (shared_ptr<Entity> actualPowerUp : manPowerUps->GetEntities())  // actualizamos los powerUp en irrlich
        physicsEngine->UpdatePowerUps(actualPowerUp.get());

    renderEngine->FacadeUpdatePlates(manNamePlates.get());
}

void StateInGame::Render() {
    renderEngine->FacadeBeginScene();
    // renderEngine->FacadeDraw();  //Para dibujar primitivas debe ir entre el drawAll y el endScene
    renderEngine->FacadeDrawAll();
    renderEngine->FacadeDrawHUD(manCars->GetCar().get(), manCars.get());
    renderEngine->FacadeDrawGraphEdges(manWayPoint.get());
    // renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), isColliding);

    for (auto actualPowerUp : manPowerUps->GetEntities())
        renderEngine->FacadeDrawBoundingBox(actualPowerUp.get(), false);

    for (auto wall : manBoundingWall->GetEntities()) {
        renderEngine->FacadeDrawBoundingPlane(wall.get());
    }

    renderEngine->FacadeDrawAIDebug(manCars.get(),manNavMesh.get());
    
    renderEngine->FacadeEndScene();
}