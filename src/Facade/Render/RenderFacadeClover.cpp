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
    string title = "BeastBrawl";
    device = new CLEngine(1280, 720, title);
    
    auto input = InputFacadeManager::GetInstance()->GetInputFacade();
    auto inputClover = static_cast<InputFacadeClover*>(input);
    inputClover->SetWindowContext(device);
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

void RenderFacadeClover::FacadeDrawHUD(Entity* car, ManCar* manCars) {
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

}

//Añade la camara, esto se llama una sola vez al crear el juego
void RenderFacadeClover::FacadeAddCamera(Entity* camera) {
    
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