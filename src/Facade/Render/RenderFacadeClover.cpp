#include "RenderFacadeClover.h"


#include <cmath>
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
#include <Components/CTargetNavMesh.h>
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
#include <Managers/ManNavMesh.h>
#include <Constants.h>
#include <Game.h>




bool RenderFacadeClover::showDebug = false;

RenderFacadeClover::~RenderFacadeClover() {
}

RenderFacadeClover::RenderFacadeClover() {
    cout << "************************************\n";
    cout << "(づ｡◕‿‿◕｡)づ   \n";
    cout << "Se inicia el motor grafico de Clover\n";
    cout << "(づ｡◕‿‿◕｡)づ   \n";
    cout << "************************************\n";

    string title = "BeastBrawl";
    device = new CLEngine(1280, 720, title);
    
    auto input = InputFacadeManager::GetInstance()->GetInputFacade();
    auto inputClover = static_cast<InputFacadeClover*>(input);
    inputClover->SetWindowContext(device);

    //Inicializamos el arbol de la escena
    smgr = device->GetSceneManager();
    resourceManager = device->GetResourceManager();


}

void RenderFacadeClover::FacadeSuscribeEvents() {
    
}

void RenderFacadeClover::FacadeInitMenu() {
    
}

void RenderFacadeClover::FacadeInitPause() {
}

void RenderFacadeClover::FacadeInitEndRace() {
}

void RenderFacadeClover::FacadeInitLobbyMulti() {
}

void RenderFacadeClover::FacadeInitHUD() {
}

void RenderFacadeClover::FacadeUpdatePowerUpHUD(DataMap* d) {
}

void RenderFacadeClover::FacadeDrawHUD(Entity* car, ManCar* carsAI) {
}

//Crea las plates de los nombres de los coches
void RenderFacadeClover::FacadeAddPlates(Manager* manNamePlates) {
}

//Actualiza las posiciones de las plates
void RenderFacadeClover::FacadeUpdatePlates(Manager* manNamePlates) {
}
const void RenderFacadeClover::FacadeAddObjects(vector<Entity*> entities) {
    for (Entity* e : entities) {
        FacadeAddObject(e);
    }
}

//INPUTS : Una entidad GameObject
//RETURNS: El Id del objeto añadido
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
    }


    if(entity->HasComponent(CompType::ShaderComp)){
        auto shader = resourceManager->GetResourceShader(cShader->vertexShader,cShader->fragmentShader);
        node->SetShaderProgramID(shader->GetProgramID());
    }

    auto prueba = smgr->GetNodeByID((unsigned int)cId->id);
    //Esto luego deberia calcular con opengl las dimensiones
    //Sacamos sus dimensiones
    float height = 10.0;
    float width = 10.0;
    float depth = 10.0;
    shared_ptr<CDimensions> cDimensions = make_shared<CDimensions>(width, height, depth);
    entity->AddComponent(cDimensions);  //Le añadimos el componente CDimensions al Entity que sea

    return cId->id;
}

//INPUTS : Una entidad GameObject
//RETURNS: El Id del objeto añadido
const uint16_t RenderFacadeClover::FacadeAddObjectCar(Entity* entity) {
    idCar = FacadeAddObject(entity);
    return idCar;
}

const uint16_t RenderFacadeClover::FacadeAddObjectTotem(Entity* entity) {
    idTotem = FacadeAddObject(entity);
    cout << "El nuevo ID de totem es: " << idTotem << "\n";
    return idTotem;
}

//TODO: Esto proximamente le pasaremos todos los entities y los modificará 1 a 1
void RenderFacadeClover::UpdateTransformable(Entity* entity) {

}

//Reajusta la camara
void RenderFacadeClover::UpdateCamera(Entity* cam, ManCar* manCars) {
    /*
    //Cogemos los componentes de la camara
    auto cTransformable = static_cast<CTransformable*>(cam->GetComponent(CompType::TransformableComp).get());
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());

    //Cogemos la posicion de nuestro coche
    glm::vec3 targetPosition = smgr->GetNodeByID(idCar)->GetTranslation();
    //core::vector3df targetRotation = smgr->getSceneNodeFromId(idCar)->getRotation();

    // Voy a calcular el punto inverso del coche, suponiendo que la camara es el centro de la circunferencia
    // Quizas luego no queremos esta manera de hacer camara trasera y me voy a cagar en vuestros putos muertos sinceramente
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
        float angleRotation = (60 * PI) / 180.0;

        cameraEntity->SetCameraTarget(targetPosition);
        //camera1->setFOV(angleRotation);
        camera1->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y-5, cTransformable->position.z));

    }else if(cCamera->camType == CamType::NORMAL_CAM){
        float angleRotation = (70 * PI) / 180.0;

        cameraEntity->SetCameraTarget(targetPosition);
        //camera1->setFOV(angleRotation);
        camera1->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));
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

        float angleRotation = (90 * PI) / 180.0;

        
        cameraEntity->SetCameraTarget(targetPosition);
        //camera1->setFOV(angleRotation);
        camera1->SetTranslation(glm::vec3(
            cTransformableCar->position.x + 32.5 * cos(valueAtan2), 
            cTransformable->position.y, 
            cTransformableCar->position.z + 35 * sin(valueAtan2)));
    }

    */
}

//Añade la camara, esto se llama una sola vez al crear el juego
void RenderFacadeClover::FacadeAddCamera(Entity* camera) {
    auto cId = static_cast<CId*>(camera->GetComponent(CompType::IdComp).get());

    auto camera1 = smgr->AddCamera(cId->id);
    auto cameraEntity = static_cast<CLCamera*>(camera1->GetEntity());

    auto cTransformable = static_cast<CTransformable*>(camera->GetComponent(CompType::TransformableComp).get());
    auto cCamera = static_cast<CCamera*>(camera->GetComponent(CompType::CameraComp).get());

    float posX = cCamera->tarX - 40.0 * sin(((cTransformable->rotation.x) * M_PI) / 180.0);
    float posZ = cCamera->tarZ - 40.0 * cos(((cTransformable->rotation.z) * M_PI) / 180.0);
    cameraEntity->SetCameraTarget(glm::vec3(cCamera->tarX, cCamera->tarY, cCamera->tarZ));
    camera1->SetTranslation(glm::vec3(posX, cTransformable->position.y, posZ));
    
}

bool RenderFacadeClover::FacadeRun() {
    return true;
}

// Devuelve el tiempo en uint32_t aunque glfw nos lo puede dar en double
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

void RenderFacadeClover::FacadeCheckInputPause() {
}

void RenderFacadeClover::FacadeCheckInputEndRace() {
}


void RenderFacadeClover::FacadeCheckInputLobbyMulti() {
}


void RenderFacadeClover::ThrowEventChangeToMulti(uint16_t IdOnline, const vector<uint16_t> IdPlayersOnline){
}


int RenderFacadeClover::FacadeGetFPS() const{
    return 1;
}

void RenderFacadeClover::FacadeSetWindowCaption(std::string title, int fps) const{
}

//Toda la rutina de limpiar y dibujar de irrlicht
void RenderFacadeClover::FacadeDraw() const{
}

void RenderFacadeClover::FacadeDrawMenu() {
}

void RenderFacadeClover::FacadeDrawPause() {
}

void RenderFacadeClover::FacadeDrawEndRace() {
}

void RenderFacadeClover::FacadeDrawLobbyMulti() {
}

//Limpia la pantalla
void RenderFacadeClover::FacadeBeginScene() const{
    device->BeginScene();
}

void RenderFacadeClover::FacadeDrawAll() const{
}

void RenderFacadeClover::FacadeEndScene() const{
    device->DrawTree();
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

void RenderFacadeClover::FacadeDrawBoundingBox(Entity* entity, bool colliding) const{
}
