#include "RenderFacadeClover.h"

#include <Constants.h>


bool RenderFacadeClover::showDebug = false;

RenderFacadeClover::~RenderFacadeClover() {
}

RenderFacadeClover::RenderFacadeClover() {
    string title = "BeastBrawl";
    device = new CLEngine(1280, 720, title);
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
}

//INPUTS : Una entidad GameObject
//RETURNS: El Id del objeto añadido
//TODO: Llevar cuidado con las rutas de las texturas si luego se mueven las carpetas
const uint16_t RenderFacadeClover::FacadeAddObject(Entity* entity) {
    return 1;
}

//INPUTS : Una entidad GameObject
//RETURNS: El Id del objeto añadido
//TODO: Llevar cuidado con las rutas de las texturas si luego se mueven las carpetas
const uint16_t RenderFacadeClover::FacadeAddObjectCar(Entity* entity) {
    return 1;
}

const uint16_t RenderFacadeClover::FacadeAddObjectTotem(Entity* entity) {
    return 1;
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

uint32_t RenderFacadeClover::FacadeGetTime() const{
    return 1;
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
}

void RenderFacadeClover::FacadeDrawAll() const{
}

void RenderFacadeClover::FacadeEndScene() const{
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
