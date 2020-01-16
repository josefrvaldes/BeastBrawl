#define _USE_MATH_DEFINES

#include "RenderFacadeIrrlicht.h"

#include <math.h>
#include "../../Aliases.h"
#include "../../Components/CBoundingPlane.h"
#include "../../Components/CBoundingSphere.h"
#include "../../Components/CPowerUp.h"
#include "../../Components/CCamera.h"
#include "../../Components/CDimensions.h"
#include "../../Components/CId.h"
#include "../../Components/CMesh.h"
#include "../../Components/CNamePlate.h"
#include "../../Components/CTexture.h"
#include "../../Components/CTotem.h"
#include "../../Components/CTransformable.h"
#include "../../Components/CType.h"
#include "../../Components/CWayPoint.h"
#include "../../Components/CWayPointEdges.h"
#include "../../Components/Component.h"
#include "../../Constants.h"
#include "../../Entities/WayPoint.h"
#include "../../Entities/CarAI.h"
#include "../../Game.h"

using namespace irr;
using namespace video;

bool RenderFacadeIrrlicht::showDebug = false;

//PUNTEROS A FUNCIONES
RenderFacadeIrrlicht::~RenderFacadeIrrlicht() {
}

RenderFacadeIrrlicht::RenderFacadeIrrlicht() {
    //auto inputFacade = InputFacadeManager::GetInstance()->GetInputFacade();
    //auto inputFacadeIrrlicht = static_cast<InputFacadeIrrlicht*>(inputFacade);
    device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1280, 720), 16, false, false, false, &receiver);
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    font = device->getGUIEnvironment()->getBuiltInFont();
    FacadeSuscribeEvents();
    FacadeInitHUD();
}

void RenderFacadeIrrlicht::FacadeSuscribeEvents() {
    EventManager::GetInstance().Suscribe(Listener{
        EventType::UPDATE_POWERUP_HUD,
        bind(&RenderFacadeIrrlicht::FacadeUpdatePowerUpHUD, this, placeholders::_1),
        "facadeUpdatePowerUpHUD"});
}

void RenderFacadeIrrlicht::FacadeInitMenu() {
    menuBG = driver->getTexture("media/main_menu.png");
    driver->makeColorKeyTexture(menuBG, core::position2d<s32>(0, 0));
}

void RenderFacadeIrrlicht::FacadeInitPause() {
    pauseBG = driver->getTexture("media/pause_screen.png");
    driver->makeColorKeyTexture(pauseBG, core::position2d<s32>(0, 0));
}

void RenderFacadeIrrlicht::FacadeInitEndRace() {
    endRaceBG = driver->getTexture("media/finish_screen.png");
    driver->makeColorKeyTexture(endRaceBG, core::position2d<s32>(0, 0));
}

void RenderFacadeIrrlicht::FacadeInitHUD() {
    //Almacenamos los iconos de powerups
    powerUps[0] = driver->getTexture("media/nonepowerup.jpg");
    powerUps[1] = driver->getTexture("media/robojorobo.jpg");
    powerUps[2] = driver->getTexture("media/nitro.jpg");
    powerUps[3] = driver->getTexture("media/pudin.jpg");
    powerUps[4] = driver->getTexture("media/escudomerluzo.jpg");
    powerUps[5] = driver->getTexture("media/telebanana.jpg");
    powerUps[6] = driver->getTexture("media/melonmolon.jpg");

    whiteBG = driver->getTexture("media/whiteBG.png");
    driver->makeColorKeyTexture(whiteBG, core::position2d<s32>(0, 0));

    driver->makeColorKeyTexture(powerUps[0], core::position2d<s32>(0, 0));
    driver->makeColorKeyTexture(powerUps[1], core::position2d<s32>(0, 0));
    driver->makeColorKeyTexture(powerUps[2], core::position2d<s32>(0, 0));
    driver->makeColorKeyTexture(powerUps[3], core::position2d<s32>(0, 0));
    driver->makeColorKeyTexture(powerUps[4], core::position2d<s32>(0, 0));
    driver->makeColorKeyTexture(powerUps[5], core::position2d<s32>(0, 0));
    driver->makeColorKeyTexture(powerUps[6], core::position2d<s32>(0, 0));

    currentPowerUp = 0;
}

void RenderFacadeIrrlicht::FacadeUpdatePowerUpHUD(DataMap d) {
    typeCPowerUp type = any_cast<typeCPowerUp>(d["typePowerUp"]);
    cout << "Facada recibe el power up: " << (int)type << endl;
    currentPowerUp = int(type);
}

void RenderFacadeIrrlicht::FacadeDrawHUD(Entity* car, ManCar* carsAI) {
    //Dibujamos el texto del tiempo que llevas el totem
    auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());

    //Dibujamos el cuadrado blanco del hud
    driver->draw2DImage(whiteBG, core::position2d<s32>(200, 50),
                        core::rect<s32>(0, 0, 100, 100), 0,
                        video::SColor(255, 255, 255, 255), false);

    //operaciones para dejarle con un solo decimal
    int time = cTotem->accumulatedTime / 100.0;
    float time2 = time / 10.0;
    core::stringw mainCarText = core::stringw("Main car   ");
    core::stringw tiempoStringw = mainCarText + core::stringw(time2);
    font->draw(tiempoStringw,
               core::rect<s32>(200, 55, 300, 200),
               video::SColor(255, 0, 0, 0));
    //Dibujamos powerUp
    driver->draw2DImage(powerUps[currentPowerUp], core::position2d<s32>(50, 50),
                        core::rect<s32>(0, 0, 100, 100), 0,
                        video::SColor(255, 255, 255, 255), false);

    int i = 0;
    core::stringw textIA = core::stringw("Car AI ");
    for (auto carAI : carsAI->GetEntitiesAI()) {
        cTotem = static_cast<CTotem*>(carAI->GetComponent(CompType::TotemComp).get());

        int time = cTotem->accumulatedTime / 100.0;
        float time2 = time / 10.0;

        core::stringw iaText = textIA + core::stringw(i) + core::stringw("  ") + core::stringw(time2);
        font->draw(iaText,
                   core::rect<s32>(200, 70 + (i * 15), 300, 300),
                   video::SColor(255, 0, 0, 0));

        i++;
    }
}

//Crea las plates de los nombres de los coches
void RenderFacadeIrrlicht::FacadeAddPlates(Manager* manNamePlates) {
    for (auto plate : manNamePlates->GetEntities()) {
        auto cId = static_cast<CId*>(plate->GetComponent(CompType::IdComp).get());

        core::stringw string = core::stringw("Car AI ") + core::stringw(numEnemyCars++);
        auto node = smgr->addTextSceneNode(font, string.c_str(), video::SColor(255, 0, 0, 0), 0, core::vector3df(200, 30, 200), -1);
        node->setID(cId->id);
    }
}

//Actualiza las posiciones de las plates
void RenderFacadeIrrlicht::FacadeUpdatePlates(Manager* manNamePlates) {
    for (auto plate : manNamePlates->GetEntities()) {
        auto cNamePlate = static_cast<CNamePlate*>(plate->GetComponent(CompType::NamePlateComp).get());
        auto cId = static_cast<CId*>(plate->GetComponent(CompType::IdComp).get());

        auto node = smgr->getSceneNodeFromId(cId->id);
        auto carAI = smgr->getSceneNodeFromId(cNamePlate->idCarAsociated);

        node->setPosition(core::vector3df(carAI->getPosition().X, carAI->getPosition().Y + 20, carAI->getPosition().Z));
    }
}
const void RenderFacadeIrrlicht::FacadeAddObjects(vector<Entity*> entities) {
    for (Entity* e : entities) {
        FacadeAddObject(e);
    }
}

//INPUTS : Una entidad GameObject
//RETURNS: El Id del objeto añadido
//TODO: Llevar cuidado con las rutas de las texturas si luego se mueven las carpetas
const uint16_t RenderFacadeIrrlicht::FacadeAddObject(Entity* entity) {
    //Fuente: https://stackoverflow.com/questions/11855018/c-inheritance-downcasting
    //Como convertir un Component en cualquier tipo de sus subclases para poder usar los metodos propios

    auto cTransformable = static_cast<CTransformable*>(entity->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    auto cTexture = static_cast<CTexture*>(entity->GetComponent(CompType::TextureComp).get());
    auto cType = static_cast<CType*>(entity->GetComponent(CompType::TypeComp).get());
    auto cMesh = static_cast<CMesh*>(entity->GetComponent(CompType::MeshComp).get());

    //Switch para añadir el tipo de objeto
    scene::ISceneNode* node = nullptr;
    std::string meshPath = "media/" + cMesh->mesh;

    // añadimos el node al sceneManager dependiendo del tipo de node que sea
    switch (cType->type) {
        case ModelType::Sphere:
            node = smgr->addSphereSceneNode();
            break;

        case ModelType::Cube:
            node = smgr->addCubeSceneNode();
            break;

        case ModelType::AnimatedMesh:
            node = smgr->addAnimatedMeshSceneNode(smgr->getMesh(meshPath.c_str()));
            break;

        case ModelType::StaticMesh:
            node = smgr->addMeshSceneNode(smgr->getMesh(meshPath.c_str()));
            break;

        case ModelType::Text:
            break;
    }

    // y ahora a ese node, le ponemos sus parámetros
    std::string path = "media/" + cTexture->texture;
    if (node) {
        node->setID(cId->id);
        node->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));
        node->setRotation(core::vector3df(cTransformable->rotation.x, cTransformable->rotation.y, cTransformable->rotation.z));
        node->setScale(core::vector3df(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));
        node->setMaterialTexture(0, driver->getTexture(path.c_str()));  //Obligado incluir el c_str() si no irrlicht no carga solo con un string
        node->setMaterialFlag(video::EMF_LIGHTING, false);

        bool hasSphere = entity->HasComponent(CompType::CompBoundingSphere);
        if (hasSphere && Constants::DEBUG_SHOW_SPHERES) {
            scene::ISceneNode* nodeSphere = smgr->addSphereSceneNode(CBoundingSphere::DEFAULT_SPHERE_RADIUS);
            nodeSphere->setID(cId->id + Component::ID_DIFFERENCE);
            nodeSphere->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));
            nodeSphere->setScale(core::vector3df(1.f, 1.f, 1.f));
            nodeSphere->setMaterialTexture(0, driver->getTexture(path.c_str()));  //Obligado incluir el c_str() si no irrlicht no carga solo con un string
            nodeSphere->setMaterialFlag(video::EMF_LIGHTING, false);
            nodeSphere->setVisible(showDebug);
        }
    }

    //Cogemos sus edges
    core::vector3df* edges = new core::vector3df[8];
    core::aabbox3df boundingBox;

    boundingBox = node->getTransformedBoundingBox();
    boundingBox.getEdges(edges);
    /*
		   /3--------/7
		  / |       / |
		 /  |      /  |
		1---------5   |
		|  /2- - -|- -6
		| /       |  /
		|/        | /
		0---------4/

	*/

    //Sacamos sus dimensiones
    float height = (edges[1].Y - edges[0].Y) * cTransformable->scale.y;
    float width = (edges[5].X - edges[1].X) * cTransformable->scale.x;
    //cout << "Altura-Anchura-Profundidad: " << width << "-" << height << "-"<< depth << endl;
    float depth = (edges[2].Z - edges[0].Z) * cTransformable->scale.z;
    shared_ptr<CDimensions> cDimensions = make_shared<CDimensions>(width, height, depth);
    entity->AddComponent(cDimensions);  //Le añadimos el componente CDimensions al Entity que sea

    delete[] edges;
    return cId->id;
}

//INPUTS : Una entidad GameObject
//RETURNS: El Id del objeto añadido
//TODO: Llevar cuidado con las rutas de las texturas si luego se mueven las carpetas
const uint16_t RenderFacadeIrrlicht::FacadeAddObjectCar(Entity* entity) {
    idCar = FacadeAddObject(entity);
    return idCar;
}

const uint16_t RenderFacadeIrrlicht::FacadeAddObjectTotem(Entity* entity) {
    idTotem = FacadeAddObject(entity);
    cout << "El nuevo ID de totem es: " << idTotem << "\n";
    return idTotem;
}

//TODO: Esto proximamente le pasaremos todos los entities y los modificará 1 a 1
void RenderFacadeIrrlicht::UpdateTransformable(Entity* entity) {
    //Cogemos los componentes de ID y CTransformable
    auto cTransformable = static_cast<CTransformable*>(entity->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());

    // Cogemos el nodo de irrlicht con el ID igual al que le hemos pasado
    scene::ISceneNode* node = smgr->getSceneNodeFromId(cId->id);

    //Actualiza la posicion del objeto de irrlicht
    node->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));

    //Actualiza la rotacion del objeto de irrlicht
    node->setRotation(core::vector3df(cTransformable->rotation.x, cTransformable->rotation.y, cTransformable->rotation.z));

    //Actualiza el escalado del objeto de irrlicht
    node->setScale(core::vector3df(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));

    bool hasSphere = entity->HasComponent(CompType::CompBoundingSphere);
    if (hasSphere && Constants::DEBUG_SHOW_SPHERES) {
        scene::ISceneNode* nodeSphere = smgr->getSceneNodeFromId(cId->id + Component::ID_DIFFERENCE);
        nodeSphere->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));
        nodeSphere->setVisible(showDebug);
        //nodeSphere->setRotation(core::vector3df(cTransformable->rotation.x, cTransformable->rotation.y, cTransformable->rotation.z));
        //nodeSphere->setScale(core::vector3df(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));
    }
}

//Reajusta la camara
void RenderFacadeIrrlicht::UpdateCamera(Entity* cam, ManCar* manCars) {
    //Cogemos los componentes de la camara
    auto cTransformable = static_cast<CTransformable*>(cam->GetComponent(CompType::TransformableComp).get());
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());

    //Cogemos la posicion de nuestro coche
    core::vector3df targetPosition = smgr->getSceneNodeFromId(idCar)->getPosition();
    //core::vector3df targetRotation = smgr->getSceneNodeFromId(idCar)->getRotation();

    // Voy a calcular el punto inverso del coche, suponiendo que la camara es el centro de la circunferencia
    // Quizas luego no queremos esta manera de hacer camara trasera y me voy a cagar en vuestros putos muertos sinceramente

    targetPosition.Y += 17;

    if(cCamera->camType == CamType::INVERTED){
        targetPosition.Y += 0;

        float distX = abs(cTransformable->position.x - targetPosition.X);
        float distZ = abs(cTransformable->position.z - targetPosition.Z);

        if(cTransformable->position.x - targetPosition.X < 0){
            targetPosition.X = targetPosition.X - (2*distX);

        }else{
            targetPosition.X = targetPosition.X + (2*distX);

        }

        if(cTransformable->position.z - targetPosition.Z < 0){
            targetPosition.Z = targetPosition.Z - (2*distZ);

        }else{
            targetPosition.Z = targetPosition.Z + (2*distZ);

        }
        float angleRotation = (60 * PI) / 180.0;

        camera1->setTarget(targetPosition);
        camera1->setFOV(angleRotation);
        camera1->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y-5, cTransformable->position.z));

    }else if(cCamera->camType == CamType::NORMAL){
        float angleRotation = (70 * PI) / 180.0;

        camera1->setTarget(targetPosition);
        camera1->setFOV(angleRotation);
        camera1->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));
    }else if (cCamera->camType == CamType::TOTEM){

        auto car = manCars->GetCar();
        auto cTotemCar = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
        auto cTransformableCar = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());

        //Si somos nosotros quien tenemos el totem ponemos camara normal
        if(cTotemCar->active){
            cCamera->camType = CamType::NORMAL;
            return;

        }

        auto idCarAIWithTotem = -1;
        //Buscamos el ID del coche que tiene el totem (en caso de tenerlo)
        for(auto carAI : manCars->GetEntitiesAI()){
            auto cTotem = static_cast<CTotem*>(carAI->GetComponent(CompType::TotemComp).get());
            if(cTotem->active){
                auto cId = static_cast<CId*>(carAI->GetComponent(CompType::IdComp).get());
                idCarAIWithTotem = cId->id;
            }
        }

        //Nadie tiene el totem
        if(idCarAIWithTotem==-1){
            //Posicion del totem en el suelo
            targetPosition = smgr->getSceneNodeFromId(idTotem)->getPosition();

        }else{
            //Posicion del coche que lleva el totem
            targetPosition = smgr->getSceneNodeFromId(idCarAIWithTotem)->getPosition();

        }

        //Calculamos el angulo hasta el totem
        float vectorX = (cTransformableCar->position.x - targetPosition.X );
        float vectorZ = (cTransformableCar->position.z - targetPosition.Z );

        float valueAtan2 = atan2(vectorZ,vectorX);

        float angleRotation = (90 * PI) / 180.0;

        
        camera1->setTarget(targetPosition);
        camera1->setFOV(angleRotation);
        camera1->setPosition(core::vector3df(
            cTransformableCar->position.x + 32.5 * cos(valueAtan2), 
            cTransformable->position.y, 
            cTransformableCar->position.z + 35 * sin(valueAtan2)));
    }
    
    

}

//Añade la camara, esto se llama una sola vez al crear el juego
void RenderFacadeIrrlicht::FacadeAddCamera(Entity* camera) {
    camera1 = smgr->addCameraSceneNode();
    device->getCursorControl()->setVisible(false);

    auto cTransformable = static_cast<CTransformable*>(camera->GetComponent(CompType::TransformableComp).get());
    auto cCamera = static_cast<CCamera*>(camera->GetComponent(CompType::CameraComp).get());

    float posX = cCamera->tarX - 40.0 * sin(((cTransformable->rotation.x) * M_PI) / 180.0);
    float posZ = cCamera->tarZ - 40.0 * cos(((cTransformable->rotation.z) * M_PI) / 180.0);
    camera1->setTarget(core::vector3df(cCamera->tarX, cCamera->tarY, cCamera->tarZ));
    camera1->setPosition(core::vector3df(posX, cTransformable->position.y, posZ));
    //camera1->setFOV(40);
    
}

bool RenderFacadeIrrlicht::FacadeRun() {
    return device->run();
}

uint32_t RenderFacadeIrrlicht::FacadeGetTime() {
    return device->getTimer()->getTime();
}

// To-Do: introducir multi input
// Comprobar inputs del teclado
void RenderFacadeIrrlicht::FacadeCheckInput() {
    EventManager &eventManager = EventManager::GetInstance();

    if (receiver.IsKeyDown(KEY_ESCAPE)) {
        device->closeDevice();
    }
    if (receiver.IsKeyDown(KEY_KEY_P)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_P});
    }
    if (receiver.IsKeyDown(KEY_KEY_0)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_0});
    }
    if (receiver.IsKeyDown(KEY_KEY_I)) {
        DataMap data;
        eventManager.AddEventMulti(Event{EventType::PRESS_I, data});
    } else if (receiver.IsKeyDown(KEY_KEY_O)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_O});
    } else {
        eventManager.AddEventMulti(Event{EventType::NO_I_O_PRESS});
    }

    if (receiver.IsKeyDown(KEY_KEY_D)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_D});
    } else if (receiver.IsKeyDown(KEY_KEY_A)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_A});
    } else {
        eventManager.AddEventMulti(Event{EventType::NO_A_D_PRESS});
    }

    // MODO DEBUG
    if (receiver.IsKeyDown(KEY_F3) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();
        showDebug = !showDebug; 

    }

    // CAMARA
    if (receiver.IsKeyDown(KEY_KEY_Q) && !invertedCam && !totemCamActive) {
        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::INVERT_CAMERA});
        invertedCam = true;

    } else if(receiver.IsKeyDown(KEY_KEY_E) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelayCamera) {
        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::TOTEM_CAMERA});
        totemCamActive = !totemCamActive;
    } else if (!receiver.IsKeyDown(KEY_KEY_Q) && !totemCamActive){
        invertedCam = false;
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});

    }

    // POWERUPS
    if (receiver.IsKeyDown(KEY_SPACE)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_SPACE});
    }

    //Cambiamos a menu
    if (receiver.IsKeyDown(KEY_F2) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();
        Game::GetInstance()->SetState(State::PAUSE);
    }
}

void RenderFacadeIrrlicht::FacadeCheckInputMenu() {
    //Cambiamos a ingame
    if (receiver.IsKeyDown(KEY_F1)) {
        numEnemyCars = 0;

        //Manera un poco cutre de resetear el CId al empezar el juego
        auto cId = make_shared<CId>();
        cId->ResetNumIds();
        auto cNavMesh = make_shared<CNavMesh>();
        cNavMesh->ResetNumIds();
        Game::GetInstance()->SetState(State::INGAME_SINGLE);
    } else if (receiver.IsKeyDown(KEY_ESCAPE)) {
        device->closeDevice();
    } else if (receiver.IsKeyDown(KEY_KEY_M)) {
        numEnemyCars = 0;

        //Manera un poco cutre de resetear el CId al empezar el juego
        auto cId = make_shared<CId>();
        cId->ResetNumIds();
        auto cNavMesh = make_shared<CNavMesh>();
        cNavMesh->ResetNumIds();
        Game::GetInstance()->SetState(State::INGAME_MULTI);
    }
}

void RenderFacadeIrrlicht::FacadeCheckInputPause() {
    //Cambiamos a ingame
    if (receiver.IsKeyDown(KEY_F3) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();
        Game::GetInstance()->SetState(State::INGAME_SINGLE);
    }

    if (receiver.IsKeyDown(KEY_F4) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();

        smgr->clear();
        EventManager::GetInstance().ClearListeners();
        EventManager::GetInstance().ClearEvents();
        Game::GetInstance()->SetState(State::MENU);
    }

    if (receiver.IsKeyDown(KEY_ESCAPE)) {
        device->closeDevice();
    }
}

void RenderFacadeIrrlicht::FacadeCheckInputEndRace() {
    if (receiver.IsKeyDown(KEY_F4) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        smgr->clear();
        EventManager::GetInstance().ClearListeners();
        EventManager::GetInstance().ClearEvents();
        Game::GetInstance()->SetState(State::MENU);
    }

    if (receiver.IsKeyDown(KEY_ESCAPE)) {
        device->closeDevice();
    }
}

int RenderFacadeIrrlicht::FacadeGetFPS() {
    return driver->getFPS();
}

void RenderFacadeIrrlicht::FacadeSetWindowCaption(std::string title) {
    //Como transformar de string a wstring (irrlicht)
    std::wstring text_aux;
    for (unsigned int i = 0; i < title.length(); ++i)
        text_aux += wchar_t(title[i]);

    const wchar_t* txt = text_aux.c_str();

    device->setWindowCaption(txt);
}

//Toda la rutina de limpiar y dibujar de irrlicht
void RenderFacadeIrrlicht::FacadeDraw() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    smgr->drawAll();  // draw the 3d scene
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDrawMenu() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    //smgr->drawAll();  // draw the 3d scene
    driver->draw2DImage(menuBG, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, 1280, 720), 0,
                        video::SColor(255, 255, 255, 255), false);
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDrawPause() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    //smgr->drawAll();  // draw the 3d scene
    driver->draw2DImage(pauseBG, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, 1280, 720), 0,
                        video::SColor(255, 255, 255, 255), false);
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDrawEndRace() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    //smgr->drawAll();  // draw the 3d scene
    driver->draw2DImage(endRaceBG, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, 1280, 720), 0,
                        video::SColor(255, 255, 255, 255), false);
    driver->endScene();
}

//Limpia la pantalla
void RenderFacadeIrrlicht::FacadeBeginScene() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
}

void RenderFacadeIrrlicht::FacadeDrawAll() {
    smgr->drawAll();  // draw the 3d scene
}

void RenderFacadeIrrlicht::FacadeEndScene() {
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDeviceDrop() {
    device->drop();
}

//DEBUG dibuja las aristas entre los nodos del grafo
void RenderFacadeIrrlicht::FacadeDrawGraphEdges(ManWayPoint* manWayPoints) {
    if (!showDebug) return;  //Si no esta activado debug retornamos

    //Recorremos todos los WayPoints del manager
    for (shared_ptr<WayPoint> way : manWayPoints->GetEntities()) {
        auto cWayPoint = static_cast<CWayPoint*>(way->GetComponent(CompType::WayPointComp).get());
        auto cWayPointEdge = static_cast<CWayPointEdges*>(way->GetComponent(CompType::WayPointEdgesComp).get());

        //Recorremos el componente CWayPointEdges->edges para ir arista a arista
        for (Edge e : cWayPointEdge->edges) {
            //Cogemos la posicion de la arista que apunta e->to
            auto cWayPoint2 = static_cast<CWayPoint*>(manWayPoints->GetEntities()[e.to]->GetComponent(CompType::WayPointComp).get());

            //Usamos un color u otro en funcion de la distancia
            if (e.cost < 300) {
                Draw3DLine(cWayPoint->position, cWayPoint2->position, 0, 0, 255);
            } else if (e.cost >= 300 && e.cost < 500) {
                Draw3DLine(cWayPoint->position, cWayPoint2->position, 0, 255, 0);
            } else if (e.cost >= 500) {
                Draw3DLine(cWayPoint->position, cWayPoint2->position, 255, 0, 0);
            }
        }
    }
}

void RenderFacadeIrrlicht::Draw3DLine(vec3& pos1, vec3& pos2) const {
    Draw3DLine(pos1, pos2, 255, 0, 0);
}

void RenderFacadeIrrlicht::Draw3DLine(vec3& pos1, vec3& pos2, uint16_t r, uint16_t g, uint16_t b) const {
    video::SMaterial m;
    m.Lighting = false;
    driver->setMaterial(m);
    driver->setTransform(video::ETS_WORLD, core::matrix4());
    core::vector3df initial = core::vector3df(pos1.x, pos1.y, pos1.z);
    core::vector3df final = core::vector3df(pos2.x, pos2.y, pos2.z);
    driver->draw3DLine(initial, final, video::SColor(255, r, g, b));
}

void RenderFacadeIrrlicht::DeleteEntity(Entity* entity) {
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    scene::ISceneNode* node = smgr->getSceneNodeFromId(cId->id);
    node->remove();
}

void RenderFacadeIrrlicht::FacadeDrawBoundingPlane(Entity* entity) const {
    if (!showDebug) return;  //Si no esta activado debug retornamos

    CBoundingPlane *plane = static_cast<CBoundingPlane*>(entity->GetComponent(CompType::CompBoundingPlane).get());
    
    vec3 &a = plane->a;
    vec3 &b = plane->b;
    vec3 &c = plane->c;
    vec3 &d = plane->d;
    
    Draw3DLine(a, b);
    Draw3DLine(b, c);
    Draw3DLine(c, d);
    Draw3DLine(d, a);
}

void RenderFacadeIrrlicht::FacadeDrawBoundingBox(Entity* entity, bool colliding) {
    /*vec3 pos(-20.f, 20.f, -300.f);
    vec3 a(pos.x, pos.y + 20.f, pos.z);
    vec3 b(pos.x + 20.f, pos.y + 20.f, pos.z);
    vec3 c(pos.x + 20.f, pos.y, pos.z);
    vec3 d(pos.x, pos.y, pos.z);
    CBoundingPlane plane(a, b, c, d);
    Draw3DLine(a, b);
    Draw3DLine(b, c);
    Draw3DLine(c, d);
    Draw3DLine(d, a);*/

    if (!showDebug) return;  //Si no esta activado debug retornamos

    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    scene::ISceneNode* node = smgr->getSceneNodeFromId(cId->id);

    //Cogemos sus edges
    core::aabbox3df boundingBox;

    boundingBox = node->getTransformedBoundingBox();

    /*
		   /3--------/7
		  / |       / |
		 /  |      /  |
		1---------5   |
		|  /2- - -|- -6
		| /       |  /
		|/        | /
		0---------4/
	*/

    video::SMaterial m;
    m.Lighting = false;
    driver->setMaterial(m);
    driver->setTransform(video::ETS_WORLD, core::matrix4());
    if (colliding) {
        driver->draw3DBox(boundingBox, video::SColor(255, 255, 0, 0));
    } else {
        driver->draw3DBox(boundingBox, video::SColor(255, 0, 255, 0));
    }
}
