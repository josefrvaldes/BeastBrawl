#include "RenderFacadeClover.h"


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
#include <Components/CPath.h>
#include <Components/CTexture.h>
#include <Components/CTotem.h>
#include <Components/CType.h>
#include <Components/CShader.h>
#include <Components/CWayPointEdges.h>
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


}

/**
 * Suscripcion de eventos
 */
void RenderFacadeClover::FacadeSuscribeEvents() {
    
}

/**
 * Inicializa las cosas para el menu
 */
void RenderFacadeClover::FacadeInitMenu() {
    
}

void RenderFacadeClover::FacadeInitControler() {
    
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
    auto cTexture = static_cast<CTexture*>(entity->GetComponent(CompType::TextureComp).get());
    auto cType = static_cast<CType*>(entity->GetComponent(CompType::TypeComp).get());
    auto cMesh = static_cast<CMesh*>(entity->GetComponent(CompType::MeshComp).get());
    auto cShader = static_cast<CShader*>(entity->GetComponent(CompType::ShaderComp).get());

    std::string meshPath = "media/" + cMesh->mesh;
    auto mesh = resourceManager->GetResourceMesh(meshPath);
    
    CLNode* node = nullptr;
    // añadimos el node al sceneManager dependiendo del tipo de node que sea
    switch (cType->type) {
        case ModelType::Sphere:
            node = smgr->AddMesh(cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);

            break;

        case ModelType::Cube:
            node = smgr->AddMesh(cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);

            break;

        case ModelType::AnimatedMesh:
            node = smgr->AddMesh(cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);

            break;

        case ModelType::StaticMesh:
            node = smgr->AddMesh(cId->id);
            break;

        case ModelType::Text:
            node = smgr->AddMesh(cId->id);
            break;
        case ModelType::Light:
            node = smgr->AddLight(cId->id);
            break;
    }


    
    auto shader = resourceManager->GetResourceShader(cShader->vertexShader,cShader->fragmentShader);
    node->SetShaderProgramID(shader->GetProgramID());

    //Seteamos valores
    node->SetTranslation(glm::vec3(cTransformable->position.x,cTransformable->position.y,-cTransformable->position.z));
    node->SetRotation(glm::vec3(cTransformable->rotation.x,Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y),cTransformable->rotation.z));
    node->SetScalation(cTransformable->scale);
    //TODO: Esto luego deberia calcular con opengl las dimensiones 
    //Sacamos sus dimensiones
    float height = 10.0;
    float width = 10.0;
    float depth = 10.0;
    shared_ptr<CDimensions> cDimensions = make_shared<CDimensions>(width, height, depth);
    entity->AddComponent(cDimensions);  //Le añadimos el componente CDimensions al Entity que sea

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

    auto cTransformableCar = static_cast<CTransformable*>(manCars->GetCar()->GetComponent(CompType::IdComp).get());

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
            targetPosition.x = targetPosition.y - (2*distX);

        }else{
            targetPosition.x = targetPosition.x + (2*distX);

        }

        if(cTransformable->position.z - targetPosition.z < 0){
            targetPosition.z = targetPosition.z - (2*distZ);

        }else{
            targetPosition.z = targetPosition.z + (2*distZ);

        }
        //float angleRotation = (60 * M_PI) / 180.0;

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

    float posX = cCamera->tarX - 40.0 * sin(((cTransformable->rotation.x) * M_PI) / 180.0);
    float posZ = cCamera->tarZ - 40.0 * cos(((cTransformable->rotation.z) * M_PI) / 180.0);
    cameraEntity->SetCameraTarget(glm::vec3(cCamera->tarX, cCamera->tarY, cCamera->tarZ));
    camera1->SetTranslation(glm::vec3(posX, cTransformable->position.y+100, posZ));
    camera1->SetRotation(glm::vec3(cTransformable->rotation.x,cTransformable->rotation.y,cTransformable->rotation.z));
    camera1->SetScalation(cTransformable->scale);

    // cameraEntity->SetCameraTarget(glm::vec3(0.0f, 0.0f, 0.0f));
    // camera1->SetTranslation(glm::vec3(0.0f,100.0f,400.0f));
    // camera1->SetRotation(glm::vec3(0.0f,0.0f,0.0f));
    // camera1->SetScalation(glm::vec3(1.0f,1.0f,1.0f));
    
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

void RenderFacadeClover::FacadeCheckInputMenu() {
}

void RenderFacadeClover::FacadeCheckInputControler() {
}

void RenderFacadeClover::FacadeCheckInputPause() {
}

void RenderFacadeClover::FacadeCheckInputEndRace() {
}


void RenderFacadeClover::FacadeCheckInputLobbyMulti() {
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
}

/**
 * Ni idea de que hace esto, creo que ni se usa
 */
void RenderFacadeClover::FacadeDraw() const{
}

/**
 * Dibuja las cosas del menu
 */
void RenderFacadeClover::FacadeDrawMenu() {
}

void RenderFacadeClover::FacadeDrawControler() {
}

/**
 * Dibuja las cosas del pause
 */
void RenderFacadeClover::FacadeDrawPause() {
}

/**
 * Dibuja las cosas del EndRace
 */
void RenderFacadeClover::FacadeDrawEndRace() {
}

/**
 * Dibuja las cosas del LobbyMulti
 */
void RenderFacadeClover::FacadeDrawLobbyMulti() {
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

//DEBUG dibuja las aristas entre los nodos del grafo
void RenderFacadeClover::FacadeDrawGraphEdges(ManWayPoint* manWayPoints) const{
}

void RenderFacadeClover::FacadeDrawAIDebug(ManCar* manCars, ManNavMesh* manNavMesh, ManWayPoint* manWayPoint) const{

}

void RenderFacadeClover::FacadeDrawAIDebugPath(Entity* carAI, ManWayPoint* manWayPoint) const{
}



void RenderFacadeClover::Draw3DLine(vec3& pos1, vec3& pos2) const {
}

void RenderFacadeClover::Draw3DLine(vec3& pos1, vec3& pos2, uint16_t r, uint16_t g, uint16_t b) const {
}

void RenderFacadeClover::DeleteEntity(Entity* entity) {
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