#include "RenderFacadeClover.h"


#include <cmath>
#include <algorithm>    // std::sort

#include <Components/CBoundingChassis.h>
#include <Components/CBoundingOBB.h>
#include <Components/CPowerUp.h>
#include <Components/CBoundingPlane.h>
#include <Components/CBoundingSphere.h>
#include <Components/CCamera.h>
#include <Components/CDimensions.h>
#include <Components/CId.h>
#include <Components/CMesh.h>
#include <Components/CNamePlate.h>
#include <Components/CPath.h>
#include <Components/CTexture.h>
#include <Components/CTotem.h>
#include <Components/CType.h>
#include <Components/CShader.h>
#include <Components/CWayPointEdges.h>
#include <Components/CLight.h>
#include <Components/CMovementType.h>
#include <Components/CNavMesh.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CCar.h>

#include <Entities/CarAI.h>
#include <Entities/CarHuman.h>

#include <Systems/Physics.h>
#include <Systems/Utils.h>

#include <Managers/ManNavMesh.h>

#include <Constants.h>

#include <Game.h>

#include <Facade/Input/InputFacadeManager.h>




RenderFacadeClover::~RenderFacadeClover() {
}

/**
 * Inicializamos la fachada de clover e inicializamos el motor creando la ventana y pasandosela a las otras fachadas
 */
RenderFacadeClover::RenderFacadeClover() {
    cout << "************************************\n";
    cout << "(づ｡◕‿‿◕｡)づ   \n";
    cout << "Se inicia el motor grafico de Clover\n";
    cout << "(づ｡◕‿‿◕｡)づ   \n";
    cout << "************************************\n";

    string title = "BeastBrawl";
    device = new CLEngine(1280, 720, title);
    
    // auto input = InputFacadeManager::GetInstance()->GetInputFacade();
    // auto inputClover = static_cast<InputFacadeClover*>(input);
    // inputClover->SetWindowContext(device);

    // auto physics = PhysicsFacadeManager::GetInstance()->GetPhysicsFacade();
    // auto physicsClover = static_cast<PhysicsFacadeClover*>(physics);
    // physicsClover->SetWindowContext(device);

    //Inicializamos el arbol de la escena
    smgr = device->GetSceneManager();
    resourceManager = device->GetResourceManager();

    FacadeInitIntro();


}

/**
 * Suscripcion de eventos
 */
void RenderFacadeClover::FacadeSuscribeEvents() {
    EventManager::GetInstance().Subscribe(Listener{
        EventType::UPDATE_POWERUP_HUD,
        bind( &RenderFacadeClover::FacadeUpdatePowerUpHUD, this, placeholders::_1 ),
        "facadeUpdatePowerUpHUD"});
}


/**
 *
 */
void RenderFacadeClover::FacadeInitIntro() {
    std::string name = "media/pauseMenu.png";
    device->GetResourceManager()->GetResourceTexture(name, true);
}

/**
 * Inicializa las cosas para el menu
 */
void RenderFacadeClover::FacadeInitMenu() {

    powerUps[0] = "media/nonepowerup.png";
    powerUps[1] = "media/robojorobo.png";
    powerUps[2] = "media/nitro.png";
    powerUps[3] = "media/pudin.png";
    powerUps[4] = "media/escudomerluzo.png";
    powerUps[5] = "media/telebanana.png";
    powerUps[6] = "media/melonmolon.png";

    device->GetResourceManager()->GetResourceTexture(powerUps[0], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[1], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[2], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[3], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[4], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[5], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[6], true);

    currentPowerUp = 0;
}

void RenderFacadeClover::FacadeInitSelectCharacter() {

}

void RenderFacadeClover::FacadeInitGameOptions() {

}

void RenderFacadeClover::FacadeInitControler() {
    
}

void RenderFacadeClover::FacadeInitCredits() {

}

/**
 * Inicializa las cosas para el pause
 */
void RenderFacadeClover::FacadeInitPause() {
}

/**
 * Inicializa las cosas para EndRace
 */
void RenderFacadeClover::FacadeInitEndRace() {
}

/**
 * Inicializa las cosas de LobbyMulti
 */
void RenderFacadeClover::FacadeInitLobbyMulti() {
}

/**
 * Inicializa las cosas del HUD
 */
void RenderFacadeClover::FacadeInitHUD() {

}

/**
 * Actualiza en el hud el icono de powerup
 * @param {CTypePowerUp}
 */
void RenderFacadeClover::FacadeUpdatePowerUpHUD(DataMap* d) {
    auto type = any_cast<typeCPowerUp>((*d)[TYPE_POWER_UP]);
    currentPowerUp = int(type);
}

/**
 * Repintamos el HUD, como el ranking por ejemplo
 * @param {coche principal, manager de coches}
 */
void RenderFacadeClover::FacadeDrawHUD(Entity* car, ManCar* manCars) {
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
    if(!manCars->GetEntities().empty()){
        //Primero vamos a meter al coche principal
        ranking_t rank{};
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

    //DIBUJAMOS CURRENTPOWERUP
    device->DrawImage2D(25.0f, 25.0f, 150.0f, 150.0f, 0.1f ,powerUps[currentPowerUp], true);

    //RANKING
    int i = 0;
    //core::stringw textIA = core::stringw("Car ");
    for (const auto& cars : manCars->GetEntities()) {

        cTotem = static_cast<CTotem*>(cars->GetComponent(CompType::TotemComp).get());

        int time = cTotem->accumulatedTime / 100.0;
        float time2 = time / 10.0;
        glm::vec3 color = glm::vec3(0.0f,0.0f,255.0f);
        if(cTotem->active){
            //Si tiene el totem voy a dibujarlo rojo por ejemplo
            color = glm::vec3(255.0f, 0.0f, 0.0f);
        }
        std::string cadena = std::to_string(cTotem->positionRanking) + ". Car " + std::to_string(i) + " - " + std::to_string(time2);
        //TODO: Problema con el origen de la lectura de letras. La madre que la pario
        float altura = (device->GetScreenHeight() - 125.0f) + ((cTotem->positionRanking-1.0f)*18.0f);
        device->RenderText2D(cadena, 200.0f, altura, 0.1f, 0.35f, color);

        i++;

    }
}

/**
 * Se llama una vez para añadir las NamePlates
 * @param {manager de nameplates}
 */
void RenderFacadeClover::FacadeAddPlates(Manager* manNamePlates) {
}

/**
 * Actualiza las posiciones de las nameplates
 */
void RenderFacadeClover::FacadeUpdatePlates(Manager* manNamePlates) {
}

/**
 * Metodo para añadir muchas entidades de golpe
 */
const void RenderFacadeClover::FacadeAddObjects(vector<Entity*> entities) {
    for (Entity* e : entities) {
        FacadeAddObject(e);
    }
}

/**
 * Añadimos el objeto al arbol de la escena
 * @return {id del objeto que pasas}
 */
const uint16_t RenderFacadeClover::FacadeAddObject(Entity* entity) {
    auto cTransformable = static_cast<CTransformable*>(entity->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    //auto cTexture = static_cast<CTexture*>(entity->GetComponent(CompType::TextureComp).get());
    auto cType = static_cast<CType*>(entity->GetComponent(CompType::TypeComp).get());
    auto cShader = static_cast<CShader*>(entity->GetComponent(CompType::ShaderComp).get());

    CLResourceMesh* mesh = nullptr;
    CLResourceMaterial* mat = nullptr;
    if(entity->HasComponent(CompType::MeshComp)){
        auto cMesh = static_cast<CMesh*>(entity->GetComponent(CompType::MeshComp).get());
        std::string currentMesh = cMesh->activeMesh;
        std::string meshPath = "media/" + currentMesh;
        mesh = resourceManager->GetResourceMesh(meshPath, false);
        mat = resourceManager->GetResourceMaterial(meshPath);
    }
    
    
    CLNode* node = nullptr;
    // añadimos el node al sceneManager dependiendo del tipo de node que sea
    switch (cType->type) {
        case ModelType::Sphere:
            node = smgr->AddMesh(cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);
            static_cast<CLMesh*>(node->GetEntity())->SetMaterial(mat);

            break;

        case ModelType::Cube:
            node = smgr->AddMesh(cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);

            break;

        case ModelType::AnimatedMesh:
            node = smgr->AddMesh(cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);
            static_cast<CLMesh*>(node->GetEntity())->SetMaterial(mat);

            break;

        case ModelType::StaticMesh:
            node = smgr->AddMesh(cId->id); 
            break;

        case ModelType::Text:
            node = smgr->AddMesh(cId->id);
            break;
        case ModelType::PointLight:
            auto cLight = static_cast<CLight*>(entity->GetComponent(CompType::LightComp).get());
            node = smgr->AddPointLight(cId->id,cLight->intensity,cLight->ambient,cLight->diffuse,cLight->specular,cLight->constant,cLight->linear,cLight->quadratic);
            //static_cast<CLLight*>(node->GetEntity())->SetLightAttributes(cLight->intensity,cLight->ambient,cLight->diffuse,cLight->specular,cLight->constant,cLight->linear,cLight->quadratic);
            break;
    } 


    
    auto shader = resourceManager->GetResourceShader(cShader->vertexShader,cShader->fragmentShader);
    node->SetShaderProgramID(shader->GetProgramID());

    //Seteamos valores
    node->SetTranslation(glm::vec3(cTransformable->position.x,cTransformable->position.y,-cTransformable->position.z));
    node->SetRotation(glm::vec3(cTransformable->rotation.x,Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y),cTransformable->rotation.z));
    node->SetScalation(cTransformable->scale);

    //auto pos = cTransformable->position;
    //cout << " la posicion donde esta la entidad es ( " << pos.x<< " , " << pos.y<< " , " <<-pos.z<< " )" << endl;
    // BOUNDING BOX
    if(cType->type != ModelType::PointLight){
        float dimAABB = node->CalculateBoundingBox();
        //Sacamos sus dimensiones
        //float height = 10.0;
        //float width = 10.0;
        //float depth = 10.0;
        // TODO: el CDimensions solo para NavMesh y para la creacion de PowerUps, no en los coches (o ya veremos)
        shared_ptr<CDimensions> cDimensions = make_shared<CDimensions>(dimAABB, dimAABB, dimAABB);
        entity->AddComponent(cDimensions);  //Le añadimos el componente CDimensions al Entity que sea
    }

    return cId->id;
}

/**
 * Añade el coche principal
 */
const uint16_t RenderFacadeClover::FacadeAddObjectCar(Entity* entity) {
    idCar = FacadeAddObject(entity);
    return idCar;
}

/**
 * Añade el totem
 */
const uint16_t RenderFacadeClover::FacadeAddObjectTotem(Entity* entity) {
    idTotem = FacadeAddObject(entity);
    return idTotem;
}


/**
 * Actualiza la camara
 * @param { camara , todos los coches para saber quien tiene el totem a seguir}
 */
void RenderFacadeClover::UpdateCamera(Entity* cam, ManCar* manCars) {
    
    
    //Cogemos los componentes de la camara
    auto cTransformable = static_cast<CTransformable*>(cam->GetComponent(CompType::TransformableComp).get());
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());

    //auto cTransformableCar = static_cast<CTransformable*>(manCars->GetCar()->GetComponent(CompType::IdComp).get());

    //Cogemos la posicion de nuestro coche
    glm::vec3 targetPosition = smgr->GetNodeByID(idCar)->GetTranslation();
    //glm::vec3 targetPosition = cTransformableCar->position;


    auto cameraEntity = static_cast<CLCamera*>(camera1->GetEntity());

    targetPosition.y += 17;

    if(cCamera->camType == CamType::INVERTED_CAM){
        targetPosition.y += 0;

        float distX = abs(cTransformable->position.x - targetPosition.x);
        float distZ = abs(cTransformable->position.z - targetPosition.z);

        if(cTransformable->position.x - targetPosition.x < 0){
            targetPosition.x = targetPosition.x - (2*distX);

        }else{
            targetPosition.x = targetPosition.x + (2*distX);

        }

        if(cTransformable->position.z - targetPosition.z < 0){
            targetPosition.z = targetPosition.z - (2*distZ);

        }else{
            targetPosition.z = targetPosition.z + (2*distZ);

        }
        //float angleRotation = (60 * M_PI) / 180.0;
        cout << cTransformable->position.x << " | " << cTransformable->position.y << " | " << cTransformable->position.z << endl;
        cameraEntity->SetCameraTarget(glm::vec3(targetPosition.x,targetPosition.y,targetPosition.z));
        cameraEntity->SetFOV(60);
        camera1->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y-5, -cTransformable->position.z));
        camera1->SetRotation(glm::vec3(cTransformable->rotation.x,Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y),cTransformable->rotation.z));


    }else if(cCamera->camType == CamType::NORMAL_CAM){
        //float angleRotation = (70 * M_PI) / 180.0;

        cameraEntity->SetCameraTarget(glm::vec3(targetPosition.x,targetPosition.y,targetPosition.z));
        
        cameraEntity->SetFOV(70);
        camera1->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y, -cTransformable->position.z));
        camera1->SetRotation(glm::vec3(cTransformable->rotation.x,Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y),cTransformable->rotation.z));

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
            targetPosition = smgr->GetNodeByID(idTotem)->GetTranslation();

        }else{
            //Posicion del coche que lleva el totem
            targetPosition = smgr->GetNodeByID(idCarAIWithTotem)->GetTranslation();

        }

        //Calculamos el angulo hasta el totem
        float vectorX = (cTransformableCar->position.x - targetPosition.x );
        float vectorZ = (cTransformableCar->position.z - targetPosition.z );

        float valueAtan2 = atan2(vectorZ,vectorX);

        //float angleRotation = (90 * M_PI) / 180.0; 

        
        cameraEntity->SetCameraTarget(glm::vec3(targetPosition.x,targetPosition.y,targetPosition.z));
        
        cameraEntity->SetFOV(90);
        camera1->SetTranslation(glm::vec3(
            cTransformableCar->position.x + 32.5 * cos(valueAtan2), 
            cTransformable->position.y, 
            -cTransformableCar->position.z + 35 * sin(valueAtan2)));

        camera1->SetRotation(glm::vec3(-cTransformable->rotation.x,Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y),-cTransformable->rotation.z));
        
    }

    
}

/**
 * Se llama una vez para crear la camara
 */
void RenderFacadeClover::FacadeAddCamera(Entity* camera) {
    auto cId = static_cast<CId*>(camera->GetComponent(CompType::IdComp).get());
    auto cShader = static_cast<CShader*>(camera->GetComponent(CompType::ShaderComp).get());

    camera1 = smgr->AddCamera(cId->id);

    //leemos y asignamos el shader
    auto shader = resourceManager->GetResourceShader(cShader->vertexShader,cShader->fragmentShader);
    camera1->SetShaderProgramID(shader->GetProgramID());

    auto cameraEntity = static_cast<CLCamera*>(camera1->GetEntity());

    auto cTransformable = static_cast<CTransformable*>(camera->GetComponent(CompType::TransformableComp).get());
    auto cCamera = static_cast<CCamera*>(camera->GetComponent(CompType::CameraComp).get());

    float posX = cCamera->tarX - 40.0 * glm::sin( glm::radians( cTransformable->rotation.x ));
    float posZ = cCamera->tarZ - 40.0 * glm::cos(glm::radians( cTransformable->rotation.z ));
    cameraEntity->SetCameraTarget(glm::vec3(cCamera->tarX, cCamera->tarY, cCamera->tarZ));
    camera1->SetTranslation(glm::vec3(posX, cTransformable->position.y+100, posZ));
    camera1->SetRotation(glm::vec3(cTransformable->rotation.x,cTransformable->rotation.y,cTransformable->rotation.z));
    camera1->SetScalation(cTransformable->scale);

    
}


void RenderFacadeClover::FacadeUpdateMeshesLoD(vector<shared_ptr<Entity>> entities) {
    for (const auto& entity : entities) {
        CId *cid = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
        auto node = smgr->GetNodeByID(cid->id);
        if(node) {
            CMesh *cMesh = static_cast<CMesh*>(entity->GetComponent(CompType::MeshComp).get());
            std::string currentMesh = cMesh->activeMesh;
            std::string meshPath = "media/" + currentMesh;
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(resourceManager->GetResourceMesh(meshPath, false));
        }
    }
}

/**
 * @return {si el juego sigue abierto o no}
 */
bool RenderFacadeClover::FacadeRun() {
    return device->Run();
}

/**
 * @return {tiempo de glfw}
 */
uint32_t RenderFacadeClover::FacadeGetTime() const{
    return device->GetTime();
}


// To-Do: introducir multi input
// Comprobar inputs del teclado
void RenderFacadeClover::FacadeCheckInputSingle() {
 }


vector<Constants::InputTypes> RenderFacadeClover::FacadeCheckInputMulti() {
    vector<Constants::InputTypes> inputs;
    return inputs;
}

void RenderFacadeClover::FacadeCheckInputIntro() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputIntro();
}


void RenderFacadeClover::FacadeCheckInputMenu() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputMenu(inputMenu, maxInputMenu);
}

void RenderFacadeClover::FacadeCheckInputSelectCharacter() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputSelectCharacter(inputSC, maxInputSC);
}

void RenderFacadeClover::FacadeCheckInputGameOptions() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputGameOptions(inputGO, maxInputGO, option);
}

void RenderFacadeClover::FacadeCheckInputControler() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputController();
}

void RenderFacadeClover::FacadeCheckInputCredits() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputCredits();
}

void RenderFacadeClover::FacadeCheckInputPause() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputPause(inputPause, maxInputPause);
}

void RenderFacadeClover::FacadeCheckInputEndRace() {
}


void RenderFacadeClover::FacadeCheckInputLobbyMulti() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputLobbyMulti();
}


void RenderFacadeClover::ThrowEventChangeToMulti(uint16_t IdOnline, const vector<uint16_t> IdPlayersOnline){
}

/**
 * Devuelve los fps a los que vamos
 */
int RenderFacadeClover::FacadeGetFPS() const{
    return 1;
}

/**
 * Pone el titulo de la ventana
 */
void RenderFacadeClover::FacadeSetWindowCaption(std::string title, int fps) const{
    std::string name = title + " - " + std::to_string(fps) + " FPS";
    device->SetTitle(name);
}

/**
 * Ni idea de que hace esto, creo que ni se usa
 */
void RenderFacadeClover::FacadeDraw() const{
}

/**
 *
 */
void RenderFacadeClover::FacadeDrawIntro() {
    std::string file = "media/intro.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);

    /*std::string text;
    //glm::vec3 color = glm::vec3(0.0f, 255.0f, 0.0f);
    glm::vec3 color[3] = {
            glm::vec3(0.0f, 255.0f, 0.0f),
            glm::vec3(0.0f, 255.0f, 0.0f),
            glm::vec3(0.0f, 255.0f, 0.0f)
    };
    color[inputMenu] = glm::vec3(255.0f, 0.0f, 0.0f);
    text = "Un jugador";
    device->RenderText2D(text, 500.0f, 400.0f, 0.05f, 1.0f, color[0]);
    text = "Multijugador";
    device->RenderText2D(text, 500.0f, 350.0f, 0.05f, 1.0f, color[1]);
    text = "Salir";
    device->RenderText2D(text, 500.0f, 300.0f, 0.05f, 1.0f, color[2]);*/
}

/**
 * Dibuja las cosas del menu
 */
void RenderFacadeClover::FacadeDrawMenu() {

    std::string file = "media/main_menu.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);

    std::string text;
    //glm::vec3 color = glm::vec3(0.0f, 255.0f, 0.0f);
    glm::vec3 color[5] = {
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f)
    };
    color[inputMenu] = glm::vec3(0.0f, 255.0f, 0.0f);
    text = "Un jugador";
    device->RenderText2D(text, 500.0f, 400.0f, 0.05f, 1.0f, color[0]);
    text = "Multijugador";
    device->RenderText2D(text, 500.0f, 350.0f, 0.05f, 1.0f, color[1]);
    text = "Controles";
    device->RenderText2D(text, 500.0f, 300.0f, 0.05f, 1.0f, color[2]);
    text = "Creditos";
    device->RenderText2D(text, 500.0f, 250.0f, 0.05f, 1.0f, color[3]);
    text = "Salir";
    device->RenderText2D(text, 500.0f, 200.0f, 0.05f, 1.0f, color[4]);

}

/**
 *
 */
 void RenderFacadeClover::FacadeDrawSelectCharacter() {
    glm::vec3 color[6] = {
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f)
    };
    glm::vec3 colorB = glm::vec3(255.0f, 0.0f, 0.0f);
    color[inputSC] = glm::vec3(0.0f, 255.0f, 0.0f);
    std::string name = "<- (B)";
    device->RenderText2D(name, 50.0f, 50.0f, 0.05f, 0.5f, colorB);
    name = "Mr Penguin";
    device->RenderText2D(name, 600.0f, 550.0f, 0.05f, 0.75f, color[0]);
    name = "Captain Sharky";
    device->RenderText2D(name, 600.0f, 425.0f, 0.05f, 0.75f, color[2]);
    name = "Kaiser Kong";
    device->RenderText2D(name, 600.0f, 300.0f, 0.05f, 0.75f, color[4]);
    name = "Deacon Dragon";
    device->RenderText2D(name, 900.0f, 550.0f, 0.05f, 0.75f, color[1]);
    name = "Mrs Baxter";
    device->RenderText2D(name, 900.0f, 425.0f, 0.05f, 0.75f, color[3]);
    name = "Cyberoctopus";
    device->RenderText2D(name, 900.0f, 300.0f, 0.05f, 0.75f, color[5]);
    name = "(A) Aceptar";
    device->RenderText2D(name, 950.0f, 50.0f, 0.05f, 0.5f, colorB);
 }

/**
 * Dibuja las cosos de las opciones de partida.
 */
void RenderFacadeClover::FacadeDrawGameOptions() {
    std::string file = "media/gameoptions.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);

    //¿TRABAJAR A NIVEL DE BIT?
    //TODO: Faltan cosas. ¿Como hago esto tio?
    glm::vec3 colorB = glm::vec3(255.0f, 0.0f, 0.0f);
    glm::vec3 colorTitle = glm::vec3(255.0f, 255.0f, 0.0f);

    glm::vec3 colorOp1[4] = {
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f)
    };
    colorOp1[inputGO[0]] = glm::vec3(0.0f, 255.0f, 0.0f);
    std::string name = "Duracion de partida";
    device->RenderText2D(name, 450.0f, 600.0f, 0.05f, 0.75f, colorTitle);
    name = "2 min";
    device->RenderText2D(name, 300.0f, 500.0f, 0.05f, 0.75f, colorOp1[0]);
    name = "3 min";
    device->RenderText2D(name, 500.0f, 500.0f, 0.05f, 0.75f, colorOp1[1]);
    name = "4 min";
    device->RenderText2D(name, 700.0f, 500.0f, 0.05f, 0.75f, colorOp1[2]);
    name = "5 min";
    device->RenderText2D(name, 900.0f, 500.0f, 0.05f, 0.75f, colorOp1[3]);

    glm::vec3 colorOp2[3] = {
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f)
    };
    colorOp2[inputGO[1]] = glm::vec3(0.0f, 255.0f, 0.0f);
    name = "Tiempo de posesion";
    device->RenderText2D(name, 450.0f, 350.0f, 0.05f, 0.75f, colorTitle);
    name = "30s";
    device->RenderText2D(name, 500.0f, 250.0f, 0.05f, 0.75f, colorOp2[0]);
    name = "45s";
    device->RenderText2D(name, 600.0f, 250.0f, 0.05f, 0.75f, colorOp2[1]);
    name = "1 min";
    device->RenderText2D(name, 700.0f, 250.0f, 0.05f, 0.75f, colorOp2[2]);

    glm::vec3 colorOp3;
    if (inputGO[2] == 0) {
        colorOp3 = glm::vec3(0.0f, 255.0f, 0.0f);
    } else {
        colorOp3 = glm::vec3(0.0f, 0.0f, 255.0f);
    }
    name = "Aceptar";
    device->RenderText2D(name, 500.0f, 150.0f, 0.05f, 1.25f, colorOp3);

    name = "---->";
    float sel[3] = { 500.0f, 250.0f, 150.0f };
    device->RenderText2D(name, 100.0f, sel[option], 0.05f, 1.0f, colorB);


    name = "<- (B)";
    device->RenderText2D(name, 50.0f, 50.0f, 0.05f, 0.5f, colorB);

}

void RenderFacadeClover::FacadeInitResources(){
    FacadeBeginScene();
    std::string file = "media/loading_screen.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
    FacadeEndScene();
    //Cargamos todas las mallas
    //Mallas
    resourceManager->GetResourceMesh("media/kart_physics.fbx");
    resourceManager->GetResourceMesh("media/kart_ia.obj");
    resourceManager->GetResourceMesh("media/melon.obj");
    resourceManager->GetResourceMesh("media/totem_tex.fbx");
    resourceManager->GetResourceMesh("media/TEST_BOX.fbx");
    resourceManager->GetResourceMesh("media/pudin.obj");
    resourceManager->GetResourceMesh("media/telebanana.obj");
    resourceManager->GetResourceMesh("media/training_ground.obj");

    //Texturas
    resourceManager->GetResourceTexture("media/escudomerluzo.png",true);
    resourceManager->GetResourceTexture("media/melonmolon.png",true);
    resourceManager->GetResourceTexture("media/nitro.png",true);
    resourceManager->GetResourceTexture("media/nonepowerup.png",true);
    resourceManager->GetResourceTexture("media/pudin.png",true);
    resourceManager->GetResourceTexture("media/robojorobo.png",true);
    resourceManager->GetResourceTexture("media/telebanana.png",true);

    //Shaders
    resourceManager->GetResourceShader("CLEngine/src/Shaders/cartoonShader.vert", "CLEngine/src/Shaders/cartoonShader.frag");

    
}

/**
 * Dibuja las cosas de los controles
 */
void RenderFacadeClover::FacadeDrawControler() {
    std::string file = "media/controller_scheme.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
}

/**
 * Dibuja las cosas del pause
 */
void RenderFacadeClover::FacadeDrawPause() {
    std::string file = "media/pause_screen.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);

    std::string text;
    glm::vec3 color[2] = {
            glm::vec3(0.0f, 0.0f, 255.0f),
            glm::vec3(0.0f, 0.0f, 255.0f)
    };
    color[inputPause] = glm::vec3(0.0f, 255.0f, 0.0f);
    text = "Continuar";
    device->RenderText2D(text, 500.0f, 400.0f, 0.05f, 1.0f, color[0]);
    text = "Salir";
    device->RenderText2D(text, 500.0f, 300.0f, 0.05f, 1.0f, color[1]);
}

/**
 * Dibuja las cosas del EndRace
 */
void RenderFacadeClover::FacadeDrawEndRace() {
    std::string file = "media/finish_screen.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
}

/**
 * Dibuja las cosas de los creditos
 */
void RenderFacadeClover::FacadeDrawCredits() {
    std::string file = "media/creditos.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
}

/**
 * Dibuja las cosas del LobbyMulti
 */
void RenderFacadeClover::FacadeDrawLobbyMulti() {
    std::string file = "media/LobbyMulti.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
}

void RenderFacadeClover::FacadeDrawLobbyMultiExit() {
}

/**
 * Limpia la pantalla y vacia los buffers de pantalla
 */
void RenderFacadeClover::FacadeBeginScene() const{
    device->BeginScene();
}

/**
 * Dibuja los objetos del arbol
 */
void RenderFacadeClover::FacadeDrawAll() const{
    device->DrawObjects();
}

/**
 * Acaba la escena, intercambiando buffers y liberando los eventos de glfw de teclado
 */
void RenderFacadeClover::FacadeEndScene() const{
    device->PollEvents();
    device->EndScene();
}

void RenderFacadeClover::FacadeDeviceDrop() {
}

void RenderFacadeClover::FacadeAddSkybox(string right,string left,string top,string bottom,string front,string back){
    smgr->AddSkybox(right, left, top, bottom, front, back);
}

void RenderFacadeClover::FacadeAddShadowMapping(unsigned int lightId){
    smgr->AddShadowMapping(lightId);
}


//DEBUG dibuja las aristas entre los nodos del grafo
void RenderFacadeClover::FacadeDrawGraphEdges(ManWayPoint* manWayPoints) const{
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
            float radians = (angle*3.1415) / 180.0;
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
                Draw3DLine(cWayPoint->position, cWayPoint2->position, 0.0,0.0,255.0);
            } else if (e.cost >= 300 && e.cost < 500) {
                Draw3DLine(cWayPoint->position, cWayPoint2->position,0.0,255.0,0.0);
            } else if (e.cost >= 500) {
                Draw3DLine(cWayPoint->position, cWayPoint2->position, 255.0,0.0,0.0);
            }
        }
    }
}

void RenderFacadeClover::FacadeDrawAIDebug(ManCar* manCars, ManNavMesh* manNavMesh, ManWayPoint* manWayPoint) const{
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
            // auto cDimensions = static_cast<CDimensions*>(carAI->GetComponent(CompType::DimensionsComp).get());
            // auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(carAI->GetComponent(CompType::CurrentNavMeshComp).get());
            //auto cTargetNavMesh = static_cast<CTargetNavMesh*>(carAI->GetComponent(CompType::TargetNavMeshComp).get());

            Draw3DLine(cPosDestination->position,cTransformableCar->position);
            //Ahora vamos a dibujar su CPath
            FacadeDrawAIDebugPath(carAI.get(),manWayPoint);

            /*
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

            auto cPath = static_cast<CPath*>(carAI->GetComponent(CompType::PathComp).get());
            auto cPathAux = stack<int>(cPath->stackPath);

            core::stringw pathText = posDestinationText + core::stringw("Path: ");
            while(!cPathAux.empty()){
                auto idWaypoint = cPathAux.top();
                pathText += core::stringw(idWaypoint) + core::stringw(" - ");
                cPathAux.pop();
            }
            pathText += core::stringw("\n");

            core::stringw navMeshText = pathText + core::stringw("Current NavMesh: ") + core::stringw(cCurrentNavMesh->currentNavMesh) + core::stringw("\n")+core::stringw("\n");
                                                //core::stringw("Target NavMesh: ")  + core::stringw(cTargetNavMesh->targetNavMesh) + 
            
            auto cMovementType = static_cast<CMovementType*>(carAI->GetComponent(CompType::MovementComp).get());

            core::stringw movementTypeText = navMeshText + core::stringw("Tipo de IA: ") + core::stringw(cMovementType->movementType.c_str()) + core::stringw("\n");

            font->draw(movementTypeText,
                core::rect<s32>(900, 55, 500, 500),
                video::SColor(255, 0, 0, 0));
            */
        }
           
    }

}

void RenderFacadeClover::FacadeDrawAIDebugPath(Entity* carAI, ManWayPoint* manWayPoint) const{
    auto cPath = static_cast<CPath*>(carAI->GetComponent(CompType::PathComp).get());

    auto cPathAux = stack<int>(cPath->stackPath);

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



void RenderFacadeClover::Draw3DLine(vec3& pos1, vec3& pos2) const {
    Draw3DLine(pos1, pos2, 255, 0, 0);
}

void RenderFacadeClover::Draw3DLine(vec3& pos1, vec3& pos2, uint16_t r, uint16_t g, uint16_t b) const {
    smgr->Draw3DLine(pos1.x,pos1.y,-pos1.z, pos2.x,pos2.y,-pos2.z, CLE::CLColor(r,g,b,255.0));
}

void RenderFacadeClover::DeleteEntity(Entity* entity) {
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    smgr->DeleteNode(cId->id);
}

void RenderFacadeClover::FacadeDrawBoundingPlane(Entity* entity) const {
}


void RenderFacadeClover::FacadeDrawBoundingOBB(Entity* entity) const {

}

void RenderFacadeClover::FacadeDrawBoundingGround(Entity* entity) const {

}

void RenderFacadeClover::FacadeDrawBoundingBox(Entity* entity, bool colliding) const{
}

void RenderFacadeClover::FacadeAddSphereOnObject(Entity* entity){
    
}

void RenderFacadeClover::CleanScene() {

    device->Clear();

}

void RenderFacadeClover::FacadeUpdateViewport(){
    device->UpdateViewport();
}