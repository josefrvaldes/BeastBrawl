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
 
 
 #include "RenderFacadeIrrlicht.h"

#define _USE_MATH_DEFINES

#include <cmath>
#include <algorithm>    // std::sort
#include "../../Components/CBoundingChassis.h" 
#include "../../Components/CBoundingOBB.h"
#include "../../Components/CPowerUp.h"

#include <Components/CBoundingPlane.h>
#include <Components/CBoundingSphere.h>
#include <Components/CCamera.h>
#include <Components/CDimensions.h>
#include <Components/CId.h>
#include <Components/CMesh.h>
#include <Components/CNamePlate.h>
#include <Components/CBrainAI.h>
#include <Components/CTexture.h>
#include <Components/CTotem.h>
#include <Components/CType.h>
#include <Components/CWayPointEdges.h>
#include <Components/CNavMesh.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CCar.h>
#include <Entities/CarAI.h>
#include <Entities/CarHuman.h>
#include <Systems/Physics.h>
#include <Managers/ManNavMesh.h>
#include <Constants.h>
#include <Game.h>

using namespace irr;
using namespace video;

// bool RenderFacadeIrrlicht::showDebug = false;
// bool RenderFacadeIrrlicht::showAIDebug = false;

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
    FacadeInitHUD();


    //core::array<SJoystickInfo> joystickInfo;
    if(device->activateJoysticks(joystickInfo))
    {
        //std::cout << "Joystick support is enabled and " << joystickInfo.size() << " joystick(s) are present." << std::endl;

        for(u32 joystick = 0; joystick < joystickInfo.size(); ++joystick)
        {
            //std::cout << "Joystick " << joystick << ":" << std::endl;
            //std::cout << "\tName: '" << joystickInfo[joystick].Name.c_str() << "'" << std::endl;
            //std::cout << "\tAxes: " << joystickInfo[joystick].Axes << std::endl;
            //std::cout << "\tButtons: " << joystickInfo[joystick].Buttons << std::endl;

            //std::cout << "\tHat is: ";
            
            switch(joystickInfo[joystick].PovHat)
            {
            case SJoystickInfo::POV_HAT_PRESENT:
                //std::cout << "present" << std::endl;
                break;

            case SJoystickInfo::POV_HAT_ABSENT:
                //std::cout << "absent" << std::endl;
                break;

            case SJoystickInfo::POV_HAT_UNKNOWN:
            default:
                //std::cout << "unknown" << std::endl;
                break;
            }
        }
    }
    //else
    //{
    //    std::cout << "Joystick support is not enabled." << std::endl;
    //}
    
    for(int i = 0; i != InputXBox::END; ++i){
        inputsPressed.emplace(static_cast<InputXBox>(i), false);
    }
}

void RenderFacadeIrrlicht::FacadeSuscribeEvents() {
    EventManager::GetInstance().Subscribe(Listener{
        EventType::UPDATE_POWERUP_HUD,
        bind(&RenderFacadeIrrlicht::FacadeUpdatePowerUpHUD, this, placeholders::_1),
        "facadeUpdatePowerUpHUD"});
}


void RenderFacadeIrrlicht::FacadeSuscribeEventsSettings() {
    
}


//////////////////////////
//  INICIA LOS MENUS    //
//////////////////////////

void RenderFacadeIrrlicht::FacadeInitIntro() {
    introBG = driver->getTexture("media/pauseMenu.png");
    driver->makeColorKeyTexture(introBG, core::position2d<s32>(0, 0));
}

void RenderFacadeIrrlicht::FacadeInitSelectCharacter() {
    
}

void RenderFacadeIrrlicht::FacadeInitGameOptions() {

}

void RenderFacadeIrrlicht::FacadeInitMenu() {
    menuBG = driver->getTexture("media/main_menu.png");
    driver->makeColorKeyTexture(menuBG, core::position2d<s32>(0, 0));
}

void RenderFacadeIrrlicht::FacadeInitControler() {
    controlerBG = driver->getTexture("media/controller_scheme.png");
    driver->makeColorKeyTexture(controlerBG, core::position2d<s32>(0, 0));
}

void RenderFacadeIrrlicht::FacadeInitCredits() {

}

void RenderFacadeIrrlicht::FacadeInitPause() {
    pauseBG = driver->getTexture("media/pause_screen.png");
    driver->makeColorKeyTexture(pauseBG, core::position2d<s32>(0, 0));
}

void RenderFacadeIrrlicht::FacadeInitEndRace() {
    endRaceBG = driver->getTexture("media/finish_screen.png");
    driver->makeColorKeyTexture(endRaceBG, core::position2d<s32>(0, 0));
}

void RenderFacadeIrrlicht::FacadeInitLobbyMulti() {
    lobbyMultFullBG = driver->getTexture("media/LobbyMultiFull.png");
    driver->makeColorKeyTexture(lobbyMultFullBG, core::position2d<s32>(0, 0));

    lobbyMultBG = driver->getTexture("media/LobbyMulti.png");
    driver->makeColorKeyTexture(lobbyMultBG, core::position2d<s32>(0, 0));
}



void RenderFacadeIrrlicht::FacadeInitHUD() {
    //Almacenamos los iconos de powerups
    powerUps[0] = driver->getTexture("media/nonepowerup.png");
    powerUps[1] = driver->getTexture("media/robojorobo.png");
    powerUps[2] = driver->getTexture("media/nitro.png");
    powerUps[3] = driver->getTexture("media/pudin.png");
    powerUps[4] = driver->getTexture("media/escudomerluzo.png");
    powerUps[5] = driver->getTexture("media/telebanana.png");
    powerUps[6] = driver->getTexture("media/melonmolon.png");

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

void RenderFacadeIrrlicht::FacadeInitSettings() {

}


void RenderFacadeIrrlicht::FacadeUpdatePowerUpHUD(DataMap* d) {
    typeCPowerUp type = any_cast<typeCPowerUp>((*d)[TYPE_POWER_UP]);
    //cout << "Facada recibe el power up: " << (int)type << endl;
    currentPowerUp = int(type);
}




void RenderFacadeIrrlicht::FacadeDrawHUD(Entity* car, ManCar* manCars, Entity* globalClock, ManHUDEvent* manHud, ManGameRules* manGR, bool ingame) {

    //Voy a actualizar aqui las posiciones donde van porque es el unico sitio donde tengo ambos tipos de coches

    //struct auxiliar para guardarme tiempo y numero de coche
    struct ranking_t{
        uint16_t carNumber;
        float    time;
        inline bool operator() (const ranking_t& struct1, const ranking_t& struct2)
        {
            return (struct1.time > struct2.time);
        }
    };

    auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
    vector<ranking_t> ranking;

    //Si existen coches de IA
    if(manCars->GetEntities().size()>0){
        //Primero vamos a meter al coche principal
        ranking_t rank;
        int i = 0;
        for(auto& carAux : manCars->GetEntities()){
            cTotem = static_cast<CTotem*>(carAux->GetComponent(CompType::TotemComp).get());
            rank.carNumber = i++;
            rank.time = cTotem->accumulatedTime;
            ranking.push_back(rank);
        }

        std::sort (ranking.begin(), ranking.end(), ranking_t());
    }

    //Ya tenemos ordenados las posiciones, ahora vamos a actualizar sus valores en el CTotem
    int j = 1;
    for(auto aux : ranking){
        uint16_t numCar = aux.carNumber;
        cTotem = static_cast<CTotem*>(manCars->GetEntities()[numCar]->GetComponent(CompType::TotemComp).get());
        cTotem->positionRanking = j++;

    }

    //Por si quieres imprimir las posiciones de los coches
    // int k = 1;
    // for(auto auxCar : manCars->GetEntities()){
    //     cTotem = static_cast<CTotem*>(auxCar->GetComponent(CompType::TotemComp).get());
    //     cout << "El coche numero " << k++ << " va en la posicion: " << cTotem->positionRanking << endl;
    // }

    //Dibujamos el texto del tiempo que llevas el totem
    cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());

    //Dibujamos el cuadrado blanco del hud
    driver->draw2DImage(whiteBG, core::position2d<s32>(200, 50),
                        core::rect<s32>(0, 0, 100, 100), 0,
                        video::SColor(255, 255, 255, 255), false);

    //operaciones para dejarle con un solo decimal
    // int time = cTotem->accumulatedTime / 100.0;
    // float time2 = time / 10.0;
    // core::stringw mainCarText = core::stringw("Main car   ");
    // core::stringw tiempoStringw = mainCarText + core::stringw(time2);
    // font->draw(tiempoStringw,
    //            core::rect<s32>(200, 55, 300, 200),
    //            video::SColor(255, 0, 0, 0));
    //Dibujamos powerUp
    driver->draw2DImage(powerUps[currentPowerUp], core::position2d<s32>(50, 25),
                        core::rect<s32>(0, 0, 125, 125), 0,
                        video::SColor(255, 255, 255, 255), true);

    int i = 0;
    core::stringw textIA = core::stringw("Car ");
    for (const auto& cars : manCars->GetEntities()) {
        
        cTotem = static_cast<CTotem*>(cars->GetComponent(CompType::TotemComp).get());

        int time = cTotem->accumulatedTime / 100.0;
        float time2 = time / 10.0;
        video::SColor color(255,0,0,0);
        if(cTotem->active){
            //Si tiene el totem voy a dibujarlo rojo por ejemplo
            color.setRed(255);
        }
        core::stringw iaText = core::stringw(cTotem->positionRanking) + core::stringw("  ") + textIA + core::stringw(i) + core::stringw("  ") + core::stringw(time2);
        font->draw(iaText,
                core::rect<s32>(200, 70 + ((cTotem->positionRanking-1) * 15), 300, 300),
                color);

        i++;
        
    }
}



//Crea las plates de los nombres de los coches
void RenderFacadeIrrlicht::FacadeAddPlates(Manager* manNamePlates) {
    for (const auto& plate : manNamePlates->GetEntities()) {
        auto cId = static_cast<CId*>(plate->GetComponent(CompType::IdComp).get());

        core::stringw string = core::stringw("Car AI ") + core::stringw(numEnemyCars++);
        auto node = smgr->addTextSceneNode(font, string.c_str(), video::SColor(255, 0, 0, 0), 0, core::vector3df(200, 30, 200), -1);
        node->setID(cId->id);
    }
}

//Actualiza las posiciones de las plates
void RenderFacadeIrrlicht::FacadeUpdatePlates(Manager* manNamePlates, Manager* manCars) {
    for (const auto& plate : manNamePlates->GetEntities()) {
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
const uint16_t RenderFacadeIrrlicht::FacadeAddObject(Entity* entity, bool maincar_) {
    return FacadeAddObject(entity, false);
}

const uint16_t RenderFacadeIrrlicht::FacadeAddObject(Entity* entity) {
    //Fuente: https://stackoverflow.com/questions/11855018/c-inheritance-downcasting
    //Como convertir un Component en cualquier tipo de sus subclases para poder usar los metodos propios

    auto cTransformable = static_cast<CTransformable*>(entity->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    auto cTexture = static_cast<CTexture*>(entity->GetComponent(CompType::TextureComp).get());
    auto cType = static_cast<CType*>(entity->GetComponent(CompType::TypeComp).get());

    //Switch para añadir el tipo de objeto
    scene::ISceneNode* node = nullptr;
    std::string meshPath = "";
    if(entity->HasComponent(CompType::MeshComp)){
        auto cMesh = static_cast<CMesh*>(entity->GetComponent(CompType::MeshComp).get());
        std::string currentMesh = cMesh->activeMesh;
        meshPath = "media/" + currentMesh;
    }

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

        case ModelType::PointLight:
            return 0;
            break;

        case ModelType::ParticleSystem:
            break;
        case ModelType::Shield:

            break;
    }

    // y ahora a ese node, le ponemos sus parámetros
    std::string path = "media/" + cTexture->texture;
    if (node) {
        node->setID(cId->id);
        node->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));
        node->setRotation(core::vector3df(cTransformable->rotation.x, cTransformable->rotation.y, cTransformable->rotation.z));
        node->setScale(core::vector3df(cTransformable->scale.x, cTransformable->scale.y, cTransformable->scale.z));
        //node->setMaterialTexture(0, driver->getTexture(path.c_str()));  //Obligado incluir el c_str() si no irrlicht no carga solo con un string
        node->setMaterialFlag(video::EMF_LIGHTING, false);

        

        
        
        bool hasChassis = entity->HasComponent(CompType::CompBoundingChassis);
        if (hasChassis && Constants::DEBUG_SHOW_CHASSIS) {
            //cout << "entramos aqui???" << endl;
            auto cChassis = static_cast<CBoundingChassis *>(entity->GetComponent(CompType::CompBoundingChassis).get());
            // primera esfera
            auto radiousSph1 = cChassis->sphereBehind->radius;
            auto centerSph1 = cChassis->sphereBehind->center;
            //cout << "POS X: " << centerSph1.x << " POS Y: " << centerSph1.y << "POS Z:" << centerSph1.z << endl;
            scene::ISceneNode* nodeSphere1 = smgr->addSphereSceneNode(radiousSph1);
            nodeSphere1->setID(cId->id + Component::ID_DIFFERENCE);
            nodeSphere1->setPosition(core::vector3df(centerSph1.x, centerSph1.y, centerSph1.z));
            nodeSphere1->setScale(core::vector3df(1.f, 1.f, 1.f));
            //nodeSphere1->setMaterialTexture(0, driver->getTexture(path.c_str()));  //Obligado incluir el c_str() si no irrlicht no carga solo con un string
            nodeSphere1->setMaterialFlag(video::EMF_LIGHTING, false);
            nodeSphere1->setVisible(false);
            // segunda esfera
            auto radiousSph2 = cChassis->sphereFront->radius;
            auto centerSph2 = cChassis->sphereFront->center;
            //cout << "POS X: " << centerSph2.x << " POS Y: " << centerSph2.y << "POS Z:" << centerSph2.z << endl;
            scene::ISceneNode* nodeSphere2 = smgr->addSphereSceneNode(radiousSph2);
            nodeSphere2->setID(cId->id + Component::ID_DIFFERENCE + Component::ID_DIFFERENCE);
            nodeSphere2->setPosition(core::vector3df(centerSph2.x, centerSph2.y, centerSph2.z));
            nodeSphere2->setScale(core::vector3df(1.f, 1.f, 1.f));
            nodeSphere2->setMaterialTexture(0, driver->getTexture(path.c_str()));  //Obligado incluir el c_str() si no irrlicht no carga solo con un string
            nodeSphere2->setMaterialFlag(video::EMF_LIGHTING, false);
            nodeSphere2->setVisible(false);
        }
        /*else{
            bool hasSphere = entity->HasComponent(CompType::CompBoundingSphere);
            if (hasSphere && Constants::DEBUG_SHOW_SPHERES) {
                auto cBoundingSphere = static_cast<CBoundingSphere *>(entity->GetComponent(CompType::CompBoundingSphere).get());
                //cout << "POS X: " << cTransformable->position.x << " POS Y: " << cTransformable->position.y << "POS Z:" << cTransformable->position.z << endl;
                cout << "lo creamos aqui con el radio de: " << cBoundingSphere->radius << endl;
                scene::ISceneNode* nodeSphere = smgr->addSphereSceneNode(cBoundingSphere->radius);
                nodeSphere->setID(cId->id + Component::ID_DIFFERENCE);
                nodeSphere->setPosition(core::vector3df(cBoundingSphere->center.x, cBoundingSphere->center.y, cBoundingSphere->center.z));
                nodeSphere->setScale(core::vector3df(1.f, 1.f, 1.f));
                nodeSphere->setMaterialTexture(0, driver->getTexture(path.c_str()));  //Obligado incluir el c_str() si no irrlicht no carga solo con un string
                nodeSphere->setMaterialFlag(video::EMF_LIGHTING, false);
                nodeSphere->setVisible(true);
            }
        }*/
        

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

void RenderFacadeIrrlicht::FacadeUpdateMeshesLoD(vector<shared_ptr<Entity>> entities) {

}

void RenderFacadeIrrlicht::FacadeUpdateAnimationsLoD(vector<shared_ptr<Entity>> entities) {

}

void RenderFacadeIrrlicht::FacadeAnimate(vector<shared_ptr<Entity>> entities) {

}

void RenderFacadeIrrlicht::Draw2DImage(float x_, float y_, int width_, int height_, float depth_, string file_, bool bool_) const {}

std::tuple<int, int> RenderFacadeIrrlicht::GetScreenSize() {return std::tuple<int, int>(0,0);}

//INPUTS : Una entidad GameObject
//TODO: Llevar cuidado con las rutas de las texturas si luego se mueven las carpetas
void RenderFacadeIrrlicht::FacadeAddSphereOnObject(Entity* entity) {
    //Fuente: https://stackoverflow.com/questions/11855018/c-inheritance-downcasting
    //Como convertir un Component en cualquier tipo de sus subclases para poder usar los metodos propios
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    std::string path = "media/";

    bool hasSphere = entity->HasComponent(CompType::CompBoundingSphere);
    if (hasSphere && Constants::DEBUG_SHOW_SPHERES) {
        auto cBoundingSphere = static_cast<CBoundingSphere *>(entity->GetComponent(CompType::CompBoundingSphere).get());
        //cout << "POS X: " << cTransformable->position.x << " POS Y: " << cTransformable->position.y << "POS Z:" << cTransformable->position.z << endl;
        //cout << "lo creamos aqui con el radio de: " << cBoundingSphere->radius << endl;
        scene::ISceneNode* nodeSphere = smgr->addSphereSceneNode(cBoundingSphere->radius);
        nodeSphere->setID(cId->id + Component::ID_DIFFERENCE);
        nodeSphere->setPosition(core::vector3df(cBoundingSphere->center.x, cBoundingSphere->center.y, cBoundingSphere->center.z));
        nodeSphere->setScale(core::vector3df(1.f, 1.f, 1.f));
        nodeSphere->setMaterialTexture(0, driver->getTexture(path.c_str()));  //Obligado incluir el c_str() si no irrlicht no carga solo con un string
        nodeSphere->setMaterialFlag(video::EMF_LIGHTING, false);
        nodeSphere->setVisible(true);
    }
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
    //cout << "El nuevo ID de totem es: " << idTotem << "\n";
    return idTotem;
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

    if(cCamera->camType == CamType::INVERTED_CAM){
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

    }else if(cCamera->camType == CamType::NORMAL_CAM){
        float angleRotation = (70 * PI) / 180.0;

        camera1->setTarget(targetPosition);
        camera1->setFOV(angleRotation);
        camera1->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));
    }else if (cCamera->camType == CamType::TOTEM_CAM){

        auto car = manCars->GetCar();
        auto cTotemCar = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
        auto cTransformableCar = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());

        //Si somos nosotros quien tenemos el totem ponemos camara normal
        if(cTotemCar->active){
            cCamera->camType = CamType::NORMAL_CAM;
            return;

        }

        auto idCarAIWithTotem = -1;
        //Buscamos el ID del coche que tiene el totem (en caso de tenerlo)
        for(const auto& cars : manCars->GetEntities()){
            if(manCars->GetCar().get() != cars.get()){
                auto cTotem = static_cast<CTotem*>(cars->GetComponent(CompType::TotemComp).get());
                if(cTotem->active){
                    auto cId = static_cast<CId*>(cars->GetComponent(CompType::IdComp).get());
                    idCarAIWithTotem = cId->id;
                }
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

    float posX = cCamera->tarX - 40.0 * sin(((cTransformable->rotation.x) * PI) / 180.0);
    float posZ = cCamera->tarZ - 40.0 * cos(((cTransformable->rotation.z) * PI) / 180.0);
    camera1->setTarget(core::vector3df(cCamera->tarX, cCamera->tarY, cCamera->tarZ));
    camera1->setPosition(core::vector3df(posX, cTransformable->position.y, posZ));
    //camera1->setFOV(40);
    
}

bool RenderFacadeIrrlicht::FacadeRun() {
    return device->run();
}

uint32_t RenderFacadeIrrlicht::FacadeGetTime() const{
    return device->getTimer()->getTime();
}




//////////////////////
//  CHECK INPUTS    //
//////////////////////

// mira si el input se encuentra pulsado
bool RenderFacadeIrrlicht::IsInputPressed(InputXBox input){
    auto mapByType = inputsPressed.find(input);
    if (mapByType != inputsPressed.end()) {
        return mapByType->second;
    }//else{
       // cout << "no encuentra el boton" << "\n";
    //}
    return true; // en caso de que no exista
}

void RenderFacadeIrrlicht::SetValueInput(InputXBox input, bool valuePressed){
    auto mapByType = inputsPressed.find(input);
    if (mapByType != inputsPressed.end()) {
        mapByType->second = valuePressed;
    }
}

void RenderFacadeIrrlicht::FacadeCheckInputIntro() {

}

void RenderFacadeIrrlicht::FacadeCheckInputSettings() {

}

void RenderFacadeIrrlicht::FacadeCheckInputSingle() {
    EventManager &eventManager = EventManager::GetInstance();

    // device->activateJoysticks(joystickInfo);

    if (receiver.IsKeyDown(KEY_DELETE)) {
        device->closeDevice();
    }
    if (receiver.IsKeyDown(KEY_KEY_P) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_STICK_R)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_P});
    }
    // if (receiver.IsKeyDown(KEY_DELETE)) {
    //     eventManager.AddEventMulti(Event{EventType::PRESS_0});
    // }

    if (receiver.IsKeyDown(KEY_KEY_U) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_X)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_SKID});
    } else {
        eventManager.AddEventMulti(Event{EventType::NOT_SKID_PRESS});
    }
    
    //  delante y detrás
    if (receiver.IsKeyDown(KEY_KEY_W) || (joystickInfo.size()>0 && receiver.GetJoyStickState().Axis[SEvent::SJoystickEvent::AXIS_V]>-15000)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_I});
    } else if (receiver.IsKeyDown(KEY_KEY_S) || (joystickInfo.size()>0 && receiver.GetJoyStickState().Axis[SEvent::SJoystickEvent::AXIS_Z]>1500)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_O});
    } else {
        eventManager.AddEventMulti(Event{EventType::NO_I_O_PRESS});
    }

    // izq y dch
    if (receiver.IsKeyDown(KEY_KEY_D) || receiver.GetJoyStickState().Axis[SEvent::SJoystickEvent::AXIS_X]>15000) {
        eventManager.AddEventMulti(Event{EventType::PRESS_D});
    } else if (receiver.IsKeyDown(KEY_KEY_A) || receiver.GetJoyStickState().Axis[SEvent::SJoystickEvent::AXIS_X]<-15000) {
        eventManager.AddEventMulti(Event{EventType::PRESS_A});
    } else {
        eventManager.AddEventMulti(Event{EventType::NO_A_D_PRESS});
    }

    // MODO DEBUG
    if (receiver.IsKeyDown(KEY_F3) && !receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();
        showDebug = !showDebug; 

    }else if(receiver.IsKeyDown(KEY_F3) && receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        showAIDebug = !showAIDebug;
    }

    //TODO: Alargar esto para cuando tengamos mas coches para debugear
    // Seleccion de coche para debugear
    if(receiver.IsKeyDown(KEY_KEY_1) && receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        idCarAIToDebug = 0;
    }else if(receiver.IsKeyDown(KEY_KEY_2) && receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        idCarAIToDebug = 1;
    }else if(receiver.IsKeyDown(KEY_KEY_3) && receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        idCarAIToDebug = 2;
    }else if(receiver.IsKeyDown(KEY_KEY_0) && receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        idCarAIToDebug = -1;
    }

    // CAMARA
    if ((receiver.IsKeyDown(KEY_KEY_I) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_LB)) && !IsInputPressed(InputXBox::BUTTON_LB) && !totemCamActive) {
        SetValueInput(InputXBox::BUTTON_LB, true);
        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::INVERT_CAMERA});
    } else if((receiver.IsKeyDown(KEY_KEY_O) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_RB))  && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelayCamera) {
        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::TOTEM_CAMERA});
        totemCamActive = !totemCamActive;
    } else if (!(receiver.IsKeyDown(KEY_KEY_I) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_LB)) && !totemCamActive){
        SetValueInput(InputXBox::BUTTON_LB, false);
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});
    }

    // POWERUPS
    if ((receiver.IsKeyDown(KEY_SPACE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_A)) && !IsInputPressed(InputXBox::BUTTON_A)){
        SetValueInput(InputXBox::BUTTON_A, true);
        eventManager.AddEventMulti(Event{EventType::PRESS_SPACE});
    }else if(!(receiver.IsKeyDown(KEY_SPACE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_A))){
        SetValueInput(InputXBox::BUTTON_A, false);
    }

    //Cambiamos a pause
    if ((receiver.IsKeyDown(KEY_ESCAPE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_START)) && !IsInputPressed(InputXBox::BUTTON_START)) {
        SetValueInput(InputXBox::BUTTON_START, true);
        eventManager.AddEventMulti(Event{EventType::STATE_PAUSE});
        //Game::GetInstance()->SetState(State::PAUSE);
    }else if(!(receiver.IsKeyDown(KEY_ESCAPE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_START))){
        SetValueInput(BUTTON_START, false);
    }
}

vector<Constants::InputTypes> RenderFacadeIrrlicht::FacadeCheckInputMulti() {
    EventManager &eventManager = EventManager::GetInstance();
    vector<Constants::InputTypes> inputs;
    inputs.reserve(4); // para evitar el funcionamiento de cómo se redimensiona
                       // por defecto un vector, como sabemos que como máximo 
                       // va a haber un máximo de 4 inputs en el vector, 
                       // le reservamos directamente ya el espacio
    if (receiver.IsKeyDown(KEY_DELETE)) {
        device->closeDevice();
    }
    if (receiver.IsKeyDown(KEY_KEY_P) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_STICK_R)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_P});
        inputs.push_back(Constants::InputTypes::CLAXON);
    }


    if (receiver.IsKeyDown(KEY_KEY_U) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_X)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_SKID});
        inputs.push_back(Constants::InputTypes::DRIFT);
    } else {
        eventManager.AddEventMulti(Event{EventType::NOT_SKID_PRESS});
    }
    // if (receiver.IsKeyDown(KEY_DELETE)) {
    //     eventManager.AddEventMulti(Event{EventType::PRESS_0});
    // }
    
    //  delante y detrás
    if (receiver.IsKeyDown(KEY_KEY_W) || (joystickInfo.size()>0 && receiver.GetJoyStickState().Axis[SEvent::SJoystickEvent::AXIS_V]>-15000)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_I});
        inputs.push_back(Constants::InputTypes::FORWARD);
    } else if (receiver.IsKeyDown(KEY_KEY_S) || (joystickInfo.size()>0 && receiver.GetJoyStickState().Axis[SEvent::SJoystickEvent::AXIS_Z]>-15000)) {
        eventManager.AddEventMulti(Event{EventType::PRESS_O});
        inputs.push_back(Constants::InputTypes::BACK);
    } else {
        eventManager.AddEventMulti(Event{EventType::NO_I_O_PRESS});
    }

    // izq y dch
    if (receiver.IsKeyDown(KEY_KEY_D) || receiver.GetJoyStickState().Axis[SEvent::SJoystickEvent::AXIS_X]>15000) {
        eventManager.AddEventMulti(Event{EventType::PRESS_D});
        inputs.push_back(Constants::InputTypes::RIGHT);
    } else if (receiver.IsKeyDown(KEY_KEY_A) || receiver.GetJoyStickState().Axis[SEvent::SJoystickEvent::AXIS_X]<-15000) {
        eventManager.AddEventMulti(Event{EventType::PRESS_A});
        inputs.push_back(Constants::InputTypes::LEFT);
    } else {
        eventManager.AddEventMulti(Event{EventType::NO_A_D_PRESS});
    }

    // MODO DEBUG
    if (receiver.IsKeyDown(KEY_F3) && !receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay) {
        timeStart = system_clock::now();
        showDebug = !showDebug; 

    }else if(receiver.IsKeyDown(KEY_F3) && receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        showAIDebug = !showAIDebug;
    }

    //TODO: Alargar esto para cuando tengamos mas coches para debugear
    // Seleccion de coche para debugear
    if(receiver.IsKeyDown(KEY_KEY_1) && receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        idCarAIToDebug = 0;
    }else if(receiver.IsKeyDown(KEY_KEY_2) && receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        idCarAIToDebug = 1;
    }else if(receiver.IsKeyDown(KEY_KEY_3) && receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        idCarAIToDebug = 2;
    }else if(receiver.IsKeyDown(KEY_BACK) && receiver.IsKeyDown(KEY_LSHIFT) && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelay){
        timeStart = system_clock::now();
        idCarAIToDebug = -1;
    }

    // CAMARA
    if ((receiver.IsKeyDown(KEY_KEY_I) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_LB)) && !IsInputPressed(InputXBox::BUTTON_LB) && !totemCamActive) {
        SetValueInput(InputXBox::BUTTON_LB, true);
        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::INVERT_CAMERA});
    } else if((receiver.IsKeyDown(KEY_KEY_O) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_RB))  && duration_cast<milliseconds>(system_clock::now() - timeStart).count()>inputDelayCamera) {
        timeStart = system_clock::now();
        eventManager.AddEventMulti(Event{EventType::TOTEM_CAMERA});
        totemCamActive = !totemCamActive;
    } else if (!(receiver.IsKeyDown(KEY_KEY_I) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_LB)) && !totemCamActive){
        SetValueInput(InputXBox::BUTTON_LB, false);
        eventManager.AddEventMulti(Event{EventType::NORMAL_CAMERA});
    }

    // POWERUPS
    if ((receiver.IsKeyDown(KEY_SPACE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_A)) && !IsInputPressed(InputXBox::BUTTON_A)){
        SetValueInput(InputXBox::BUTTON_A, true);
        eventManager.AddEventMulti(Event{EventType::PRESS_SPACE});
        inputs.push_back(Constants::InputTypes::LAUNCH_PU);
    }else if(!(receiver.IsKeyDown(KEY_SPACE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_A))){
        SetValueInput(InputXBox::BUTTON_A, false);
    }

    //No debe tener menu pausa


    return inputs;
}

void RenderFacadeIrrlicht::FacadeCheckInputSelectCharacter() {

}

void RenderFacadeIrrlicht::FacadeCheckInputGameOptions() {

}

void RenderFacadeIrrlicht::FacadeCheckInputMenu() {
    //Cambiamos a ingame
    if ((receiver.IsKeyDown(KEY_SPACE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_A)) && !IsInputPressed(InputXBox::BUTTON_A)){
        SetValueInput(InputXBox::BUTTON_A, true);
        numEnemyCars = 0;

        //Manera un poco cutre de resetear el CId al empezar el juego
        auto cId = make_shared<CId>();
        cId->ResetNumIds();
        auto cNavMesh = make_shared<CNavMesh>();
        cNavMesh->ResetNumIds();
        //Game::GetInstance()->SetState(State::INGAME_SINGLE);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INGAMESINGLE});
    } else if ((receiver.IsKeyDown(KEY_DELETE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_BACK)) && !IsInputPressed(InputXBox::BUTTON_BACK)) {
        SetValueInput(InputXBox::BUTTON_BACK, true);
        device->closeDevice();
    } else if ((receiver.IsKeyDown(KEY_KEY_M) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_Y)) && !IsInputPressed(InputXBox::BUTTON_Y)) {
        smgr->clear();
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_LOBBYMULTI});
    }else if ((receiver.IsKeyDown(KEY_KEY_U) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_X)) && !IsInputPressed(InputXBox::BUTTON_X)) {
        smgr->clear();
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_CONTROLS});
    }

    if(!(receiver.IsKeyDown(KEY_SPACE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_A)))
        SetValueInput(InputXBox::BUTTON_A, false);
    if(!(receiver.IsKeyDown(KEY_ESCAPE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_BACK)))
        SetValueInput(InputXBox::BUTTON_BACK, false);
    if(!(receiver.IsKeyDown(KEY_KEY_M) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_Y)))
        SetValueInput(InputXBox::BUTTON_Y, false);
    if(!(receiver.IsKeyDown(KEY_KEY_U) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_X)))
        SetValueInput(InputXBox::BUTTON_X, false);
}

void RenderFacadeIrrlicht::FacadeCheckInputControler() {
    if ((receiver.IsKeyDown(KEY_KEY_N) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_B)) && !IsInputPressed(InputXBox::BUTTON_B)) {
        SetValueInput(InputXBox::BUTTON_B, true);
        smgr->clear();
        //EventManager::GetInstance().ClearListeners();
        //EventManager::GetInstance().ClearEvents();
        //Game::GetInstance()->SetState(State::MENU);
        //cout << "ENTRAAAAAA pantalla\n";
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    }else if(!(receiver.IsKeyDown(KEY_KEY_N) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_B))){
        SetValueInput(InputXBox::BUTTON_B, false);
    }

    if (receiver.IsKeyDown(KEY_DELETE)) {
        device->closeDevice();
    }
}

void RenderFacadeIrrlicht::FacadeCheckInputCredits() {

}

void RenderFacadeIrrlicht::FacadeCheckInputPause() {
    //Cambiamos a ingame
    if ((receiver.IsKeyDown(KEY_ESCAPE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_START)) && !IsInputPressed(InputXBox::BUTTON_START)) {
        SetValueInput(InputXBox::BUTTON_START, true);
        Game::GetInstance()->SetState(State::INGAME_SINGLE);
    }else if(!(receiver.IsKeyDown(KEY_ESCAPE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_START))){
        SetValueInput(InputXBox::BUTTON_START, false);
    }

    if ((receiver.IsKeyDown(KEY_KEY_N) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_B)) && !IsInputPressed(InputXBox::BUTTON_B)) {
        SetValueInput(InputXBox::BUTTON_B, true);

        smgr->clear();
        //EventManager::GetInstance().ClearListeners();
        //EventManager::GetInstance().ClearEvents();
        Game::GetInstance()->SetState(State::MENU);
        //EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    }else if(!(receiver.IsKeyDown(KEY_KEY_N) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_B))){
        SetValueInput(InputXBox::BUTTON_B, false);
    }

    if (receiver.IsKeyDown(KEY_DELETE)) {
        device->closeDevice();
    }
}

void RenderFacadeIrrlicht::FacadeCheckInputEndRace() {
    if ((receiver.IsKeyDown(KEY_SPACE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_A)) && !IsInputPressed(InputXBox::BUTTON_A)) {
        SetValueInput(InputXBox::BUTTON_A, true);
        smgr->clear();
        //EventManager::GetInstance().ClearListeners();
        //EventManager::GetInstance().ClearEvents();
        //Game::GetInstance()->SetState(State::MENU);
        //cout << "ENTRAAAAAA ENDRACE\n";
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    }else if(!(receiver.IsKeyDown(KEY_SPACE) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_A))){
        SetValueInput(InputXBox::BUTTON_A, false);
    }

    if (receiver.IsKeyDown(KEY_DELETE)) {
        device->closeDevice();
    }
}

void RenderFacadeIrrlicht::FacadeCheckInputLobbyMultiConnecting() {
    //Cambiamos a ingame
    if (receiver.IsKeyDown(KEY_DELETE)) {
        device->closeDevice();
    }

    if ((receiver.IsKeyDown(KEY_KEY_N) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_B)) && !IsInputPressed(InputXBox::BUTTON_B)) {
        SetValueInput(InputXBox::BUTTON_B, true);
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    }else if(!(receiver.IsKeyDown(KEY_KEY_N) || receiver.GetJoyStickState().IsButtonPressed(InputXBox::BUTTON_B))){
        SetValueInput(InputXBox::BUTTON_B, false);
    }
}


void RenderFacadeIrrlicht::ThrowEventChangeToMulti(uint16_t IdOnline, const vector<uint16_t> IdPlayersOnline){
    numEnemyCars = 0;
    //Manera un poco cutre de resetear el CId al empezar el juego
    auto cId = make_shared<CId>();
    cId->ResetNumIds();
    auto cNavMesh = make_shared<CNavMesh>();
    cNavMesh->ResetNumIds();
    //Game::GetInstance()->SetState(State::INGAME_MULTI);
    shared_ptr<DataMap> data = make_shared<DataMap>();
    //(*data)[DataType::DATA_SERVER] = dataServer;
    (*data)[DataType::ID_ONLINE] = IdOnline;
    (*data)[DataType::VECTOR_ID_ONLINE] = IdPlayersOnline;
    EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INGAMEMULTI, data});
}


void RenderFacadeIrrlicht::ResetInputGameOptions() {

}

void RenderFacadeIrrlicht::ResetInputCharacter() {

}



int RenderFacadeIrrlicht::FacadeGetFPS() const{
    return driver->getFPS();
}

void RenderFacadeIrrlicht::FacadeSetWindowCaption(std::string title, int fps) const{
    
    std::string name = title + " - FPS: " + std::to_string(fps);
    
    //Como transformar de string a wstring (irrlicht)
    std::wstring text_aux;
    for (unsigned int i = 0; i < name.length(); ++i)
        text_aux += wchar_t(name[i]);

    const wchar_t* txt = text_aux.c_str();

    device->setWindowCaption(txt);
}


//////////////
//  DRAW    //
//////////////

//Toda la rutina de limpiar y dibujar de irrlicht
void RenderFacadeIrrlicht::FacadeDraw() const{
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    smgr->drawAll();  // draw the 3d scene
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDrawIntro() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    driver->draw2DImage(introBG, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, 1280, 720), 0,
                        video::SColor(255, 255, 255, 255), false);
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDrawSelectCharacter() {

}

void RenderFacadeIrrlicht::FacadeDrawGameOptions() {

}

void RenderFacadeIrrlicht::FacadeDrawMenu() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    //smgr->drawAll();  // draw the 3d scene
    driver->draw2DImage(menuBG, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, 1280, 720), 0,
                        video::SColor(255, 255, 255, 255), false);
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeInitResources(mainCharacter character){

}

void RenderFacadeIrrlicht::FacadeDrawControler() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    //smgr->drawAll();  // draw the 3d scene
    driver->draw2DImage(controlerBG, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, 1280, 720), 0,
                        video::SColor(255, 255, 255, 255), false);
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDrawCredits() {

}

void RenderFacadeIrrlicht::FacadeDrawPause() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    //smgr->drawAll();  // draw the 3d scene
    driver->draw2DImage(pauseBG, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, 1280, 720), 0,
                        video::SColor(255, 255, 255, 255), false);
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDrawSettings() {

}

void RenderFacadeIrrlicht::FacadeDrawEndRace() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    //smgr->drawAll();  // draw the 3d scene
    driver->draw2DImage(endRaceBG, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, 1280, 720), 0,
                        video::SColor(255, 255, 255, 255), false);
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDrawLobbyMultiConnecting() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    //smgr->drawAll();  // draw the 3d scene
    driver->draw2DImage(lobbyMultBG, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, 1280, 720), 0,
                        video::SColor(255, 255, 255, 255), false);
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDrawLobbyMultiExit() {
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
    //smgr->drawAll();  // draw the 3d scene
    driver->draw2DImage(lobbyMultFullBG, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, 1280, 720), 0,
                        video::SColor(255, 255, 255, 255), false);
    driver->endScene();
}




//Limpia la pantalla
void RenderFacadeIrrlicht::FacadeBeginScene() const{
    driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
}

void RenderFacadeIrrlicht::FacadeDrawAll() const{
    smgr->drawAll();  // draw the 3d scene
}

void RenderFacadeIrrlicht::FacadeEndScene() const{
    driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDeviceDrop() {
    device->drop();
}

void RenderFacadeIrrlicht::FacadeAddSkybox(string right,string left,string top,string bottom,string front,string back){
    
}

void RenderFacadeIrrlicht::FacadeAddShadowMapping(unsigned int lightId){}


//DEBUG dibuja las aristas entre los nodos del grafo
void RenderFacadeIrrlicht::FacadeDrawGraphEdges(ManWayPoint* manWayPoints) const{
    if (!showDebug) return;  //Si no esta activado debug retornamos

    //Recorremos todos los WayPoints del manager
    for (const auto& way : manWayPoints->GetEntities()) {
        auto cWayPoint = static_cast<CWayPoint*>(way->GetComponent(CompType::WayPointComp).get());
        auto cWayPointEdge = static_cast<CWayPointEdges*>(way->GetComponent(CompType::WayPointEdgesComp).get());

        //Vamos a dibujar varias lineas para formar un "circulo"
        auto centre = cWayPoint->position;
        //La primera posicion es para el primer cuadrante angle 0
        auto radious = cWayPoint->radious;
        glm::vec3 lastPoint = glm::vec3(centre.x + radious,centre.y, centre.z);
        float angle = 0;
        float angleIncrement = 18; //Si quieres aumentar la precision debes bajar el numero y que siga siendo multiplo de 360

        while(angle<=360){
            angle += angleIncrement;
            float radians = (angle*PI) / 180.0;
            auto newPoint = glm::vec3(centre.x + (cos(radians) * radious), centre.y, centre.z + (sin(radians) * radious));

            Draw3DLine(lastPoint,newPoint);
            lastPoint = newPoint;
        }
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

void RenderFacadeIrrlicht::FacadeDrawAIDebug(ManCar* manCars, ManNavMesh* manNavMesh, ManWayPoint* manWayPoint) const{
    if(!showAIDebug) return;


    /*
		   /7--------/6
		  / |       / |
		 /  |      /  |
		4---------5   |
		|  /3- - -|- -2
		| /       |  /
		|/        | /
		0---------1/

        width  = Z
        depth  = X
        height = Y

	*/

    glm::vec3 point0;
    glm::vec3 point1;
    glm::vec3 point2;
    glm::vec3 point3;
    glm::vec3 point4;
    glm::vec3 point5;
    glm::vec3 point6;
    glm::vec3 point7;
    //Dibujamos el cuadrado que engloba a cada NavMesh

    for(const auto& navMesh : manNavMesh->GetEntities()){
        auto cTransformable = static_cast<CTransformable*>(navMesh->GetComponent(CompType::TransformableComp).get());
        auto cDimensions    = static_cast<CDimensions*>(navMesh->GetComponent(CompType::DimensionsComp).get());

        point0 = glm::vec3(cTransformable->position.x - (cDimensions->width/2),
                            cTransformable->position.y - (cDimensions->height/2),
                            cTransformable->position.z - (cDimensions->depth/2));

        point1 = glm::vec3(cTransformable->position.x - (cDimensions->width/2),
                            cTransformable->position.y - (cDimensions->height/2),
                            cTransformable->position.z + (cDimensions->depth/2));

        point2 = glm::vec3(cTransformable->position.x + (cDimensions->width/2),
                            cTransformable->position.y - (cDimensions->height/2),
                            cTransformable->position.z + (cDimensions->depth/2));
                            
        point3 = glm::vec3(cTransformable->position.x + (cDimensions->width/2),
                            cTransformable->position.y - (cDimensions->height/2),
                            cTransformable->position.z - (cDimensions->depth/2));

        point4 = glm::vec3(point0.x,point0.y + cDimensions->height, point0.z);
        point5 = glm::vec3(point1.x,point1.y + cDimensions->height, point1.z);
        point6 = glm::vec3(point2.x,point2.y + cDimensions->height, point2.z);
        point7 = glm::vec3(point3.x,point3.y + cDimensions->height, point3.z);

        Draw3DLine(point0,point1,0,0,0);
        Draw3DLine(point1,point2,0,0,0);
        Draw3DLine(point2,point3,0,0,0);
        Draw3DLine(point3,point0,0,0,0);
        Draw3DLine(point4,point0,0,0,0);
        Draw3DLine(point4,point5,0,0,0);
        Draw3DLine(point4,point7,0,0,0);
        Draw3DLine(point5,point1,0,0,0);
        Draw3DLine(point5,point6,0,0,0);
        Draw3DLine(point6,point2,0,0,0);
        Draw3DLine(point6,point7,0,0,0);
        Draw3DLine(point7,point3,0,0,0);

    }


    //Ahora vamos a hacer el debug desde nuestra posicion al CPosDestination
    if(idCarAIToDebug==-1){
        //Significa que lo hacemos para todos los coches
        for(const auto& carAI : manCars->GetEntities()){
            if (static_cast<Car*>(carAI.get())->GetTypeCar() == TypeCar::CarAI){
                auto cPosDestination = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
                auto cTransformableCar = static_cast<CTransformable*>(carAI->GetComponent(CompType::TransformableComp).get());

                Draw3DLine(cPosDestination->position,cTransformableCar->position);
                //Ahora vamos a dibujar su CPath
                FacadeDrawAIDebugPath(carAI.get(),manWayPoint);

                //Ahora por ultimo en la esquina superior derecha escribimos strings con datos
            }
        }
    }else{
        //Si tenemos seleccionado un coche en concreto dibujamos solo el suyo
        auto carAI = manCars->GetEntities()[idCarAIToDebug+1]; //Cogemos el coche que vamos a debugear
        if (static_cast<Car*>(carAI.get())->GetTypeCar() == TypeCar::CarAI){
            auto cPosDestination = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
            auto cTransformableCar = static_cast<CTransformable*>(carAI->GetComponent(CompType::TransformableComp).get());
            auto cDimensions = static_cast<CDimensions*>(carAI->GetComponent(CompType::DimensionsComp).get());
            auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(carAI->GetComponent(CompType::CurrentNavMeshComp).get());
            //auto CBrainAI = static_cast<CBrainAI*>(carAI->GetComponent(CompType::BrainAIComp).get());

            Draw3DLine(cPosDestination->position,cTransformableCar->position);
            //Ahora vamos a dibujar su CPath
            FacadeDrawAIDebugPath(carAI.get(),manWayPoint);

            //Ahora por ultimo en la esquina superior derecha escribimos strings con datos
            auto cCar = static_cast<CCar*>(carAI->GetComponent(CompType::CarComp).get());        
            core::stringw transfomableText = core::stringw("Post - Rot - Scale: (") + 
                                core::stringw(cTransformableCar->position.x) + core::stringw(" | ") +
                                core::stringw(cTransformableCar->position.y) + core::stringw(" | ") +
                                core::stringw(cTransformableCar->position.z) + core::stringw(")\n(") +
                                core::stringw(cTransformableCar->rotation.x) + core::stringw(" | ") +
                                core::stringw(cTransformableCar->rotation.y) + core::stringw(" | ") +
                                core::stringw(cTransformableCar->rotation.z) + core::stringw(")\n(") +
                                core::stringw(cTransformableCar->scale.x)    + core::stringw(" | ") +
                                core::stringw(cTransformableCar->scale.y)    + core::stringw(" | ") +
                                core::stringw(cTransformableCar->scale.z)    + core::stringw(")\n") ;

            core::stringw dimensionsText = transfomableText + core::stringw("Dimensions: ") + core::stringw(cDimensions->width) + core::stringw(" | ")+ 
                                                            core::stringw(cDimensions->height) + core::stringw(" | ") + 
                                                            core::stringw(cDimensions->depth) + core::stringw("\n");

            core::stringw carText = dimensionsText + core::stringw("Speed: ") + core::stringw(cCar->speed) +core::stringw("\n");
            core::stringw posDestinationText = carText + core::stringw("Destination: ") + core::stringw(cPosDestination->position.x) +core::stringw(" | ") +
                                                        core::stringw(cPosDestination->position.y) +core::stringw(" | ") + 
                                                        core::stringw(cPosDestination->position.z) +core::stringw(" \n ");

            auto cBrainAI = static_cast<CBrainAI*>(carAI->GetComponent(CompType::BrainAIComp).get());
            auto cPathAux = stack<int>(cBrainAI->stackPath);

            core::stringw pathText = posDestinationText + core::stringw("Path: ");
            while(!cPathAux.empty()){
                auto idWaypoint = cPathAux.top();
                pathText += core::stringw(idWaypoint) + core::stringw(" - ");
                cPathAux.pop();
            }
            pathText += core::stringw("\n");

            core::stringw navMeshText = pathText + core::stringw("Current NavMesh: ") + core::stringw(cCurrentNavMesh->currentNavMesh) + core::stringw("\n")+core::stringw("\n");
                                                //core::stringw("Target NavMesh: ")  + core::stringw(CBrainAI->targetNavMesh) + 
            

            core::stringw movementTypeText = navMeshText + core::stringw("Tipo de IA: ") + core::stringw(cBrainAI->movementType.c_str()) + core::stringw("\n");

            font->draw(movementTypeText,
                core::rect<s32>(900, 55, 500, 500),
                video::SColor(255, 0, 0, 0));
        }
        
    }

}

void RenderFacadeIrrlicht::FacadeDrawAIDebugPath(Entity* carAI, ManWayPoint* manWayPoint) const{
    auto cBrainAI = static_cast<CBrainAI*>(carAI->GetComponent(CompType::BrainAIComp).get());

    auto cPathAux = stack<int>(cBrainAI->stackPath);

    auto lastWaypoint = -1;
    if(!cPathAux.empty()){
        //Hago esto para dibujar la primera linea desde CPosDestination hsata el primer nodo
        auto cPosDestination = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());

        auto idWaypoint = cPathAux.top();
        auto node = manWayPoint->GetEntities()[idWaypoint];
        auto cWayPoint = static_cast<CWayPoint*>(node->GetComponent(CompType::WayPointComp).get());
        Draw3DLine(cPosDestination->position,cWayPoint->position);
        cPathAux.pop();

        //Ahora dibujamos el resto de path si queda
        while(!cPathAux.empty()){
            lastWaypoint = cWayPoint->id; //Me tengo que ir guardando el nodo anterior para dibujar desde ese al proximo
            auto lastNode = manWayPoint->GetEntities()[lastWaypoint];
            auto idWaypoint = cPathAux.top();
            auto node = manWayPoint->GetEntities()[idWaypoint];

            auto cWayPointLast = static_cast<CWayPoint*>(lastNode->GetComponent(CompType::WayPointComp).get());
            auto cWayPoint = static_cast<CWayPoint*>(node->GetComponent(CompType::WayPointComp).get());
            Draw3DLine(cWayPointLast->position,cWayPoint->position);
            cPathAux.pop();
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

// TODO: El id del compoennte sphere debe crearse en el porpio compoentne y no en el render Fachade o Physics fachade
void RenderFacadeIrrlicht::DeleteEntity(Entity* entity) {
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    // si tenemos el compoennte Sphere hay que eliminar... su Bounding
    //if (entity->HasComponent(CompType::CompBoundingSphere)){
    //    scene::ISceneNode* nodeSphere = smgr->getSceneNodeFromId(cId->id + Component::ID_DIFFERENCE);
    //    nodeSphere->remove();
    //}
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

void RenderFacadeIrrlicht::FacadeDrawBoundingGround(Entity* entity) const {
    if (!showDebug) return;  //Si no esta activado debug retornamos

    CBoundingPlane *plane = static_cast<CBoundingPlane*>(entity->GetComponent(CompType::CompBoundingPlane).get());
    
    vec3 &a = plane->a;
    vec3 &b = plane->b;
    vec3 &c = plane->c;
    vec3 &d = plane->d;
    
    Draw3DLine(a, b, 0, 0, 0);
    Draw3DLine(b, c, 0, 0, 0);
    Draw3DLine(c, d, 0, 0, 0);
    Draw3DLine(d, a, 0, 0, 0);
}


void RenderFacadeIrrlicht::FacadeDrawBoundingOBB(Entity* entity) const {
    if (!showDebug) return;  //Si no esta activado debug retornamos

    CBoundingOBB *obb = static_cast<CBoundingOBB*>(entity->GetComponent(CompType::CompBoundingOBB).get());
    
    for(auto& obbActual : obb->planes){
        vec3 &a = obbActual.get()->a;
        vec3 &b = obbActual.get()->b;
        vec3 &c = obbActual.get()->c;
        vec3 &d = obbActual.get()->d;

        Draw3DLine(a, b, 0, 0, 200);
        Draw3DLine(b, c, 0, 0, 200);
        Draw3DLine(c, d, 0, 0, 200);
        Draw3DLine(d, a, 0, 0, 200);
    }

}

void RenderFacadeIrrlicht::FacadeDrawBoundingBox(Entity* entity, bool colliding) const{
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


void RenderFacadeIrrlicht::CleanScene() {
    smgr->clear();
}

void RenderFacadeIrrlicht::FacadeUpdateViewport(){
    //device->UpdateViewport();
}

void RenderFacadeIrrlicht::SetCamTarget(glm::vec3 pos) {
    
};