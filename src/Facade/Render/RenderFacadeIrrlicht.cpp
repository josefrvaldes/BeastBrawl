#include "RenderFacadeIrrlicht.h"

#include "../../Components/CPosition.h"
#include "../../Components/CType.h"
#include "../../Components/CId.h"
#include "../../Components/CTexture.h"
#include "../../Components/CMesh.h"
#include "../../Components/CTransformable.h"
#include "../../Components/CCamera.h"
#include "../../Components/Component.h"
#include <math.h>

#define PI 3.14159

//PUNTEROS A FUNCIONES


RenderFacadeIrrlicht::RenderFacadeIrrlicht(){
	eventManager = EventManager::GetInstance();

	auto inputFacade = InputFacadeManager::GetInstance()->GetInputFacade();
	auto inputFacadeIrrlicht = static_cast<InputFacadeIrrlicht*>(inputFacade);	
	device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1280, 720), 16, false, false, false, &receiver);
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
}

const void RenderFacadeIrrlicht::FacadeAddObjects(vector<Entity*> entities) {
	for(Entity *e : entities) {
		FacadeAddObject(e);
	} 
}

//INPUTS : Una entidad GameObject
//RETURNS: El Id del objeto añadido
//TODO: Llevar cuidado con las rutas de las texturas si luego se mueven las carpetas
const uint16_t RenderFacadeIrrlicht::FacadeAddObject(Entity *go){

	//Fuente: https://stackoverflow.com/questions/11855018/c-inheritance-downcasting
	//Como convertir un Component en cualquier tipo de sus subclases para poder usar los metodos propios
	auto components = go->GetComponents();

	//TODO: Encontrar una mejor manera para acceder a los componentes ya que asi se tarda demasiado
	auto mapTransformable = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransformable->second);

	auto mapId = components.find(CompType::IdComp);
	auto cId = static_cast<CId*>(mapId->second);

	auto mapTexture = components.find(CompType::TextureComp);
	auto cTexture = static_cast<CTexture*>(mapTexture->second);

	auto mapType = components.find(CompType::TypeComp);
	auto cType 	 = static_cast<CType*>(mapType->second);

	auto mapMesh = components.find(CompType::MeshComp);
	auto cMesh	 = static_cast<CMesh*>(mapMesh->second);


	//Switch para añadir el tipo de objeto
	scene::ISceneNode* node;
	std::string meshPath = "media/" + cMesh->mesh;

	switch(cType->type){
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
	}

	std::string path = "media/" + cTexture->texture;
	if(node){
		node->setID(cId->id);
		node->setPosition(core::vector3df(cTransformable->posX, cTransformable->posY, cTransformable->posZ));
		node->setRotation(core::vector3df(cTransformable->rotX, cTransformable->rotY, cTransformable->rotZ));
		node->setScale(core::vector3df(cTransformable->scaleX, cTransformable->scaleY, cTransformable->scaleZ));
		node->setMaterialTexture(0, driver->getTexture(path.c_str())); //Obligado incluir el c_str() si no irrlicht no carga solo con un string
		node->setMaterialFlag(video::EMF_LIGHTING, false);

	}



	return cId->id;
}

//TODO: Esto proximamente le pasaremos todos los entities y los modificará 1 a 1  
void RenderFacadeIrrlicht::UpdateTransformable(Entity* go){
	//Cogemos los componentes de ID y CTransformable 
	auto components = go->GetComponents();
	auto mapTransformable = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransformable->second);
	auto mapId = components.find(CompType::IdComp);
	auto cId = static_cast<CId*>(mapId->second);

	// Cogemos el nodo de irrlicht con el ID igual al que le hemos pasado
	scene::ISceneNode* node = smgr->getSceneNodeFromId(cId->id);

	//Actualiza la posicion del objeto de irrlicht
	node->setPosition(core::vector3df(cTransformable->posX, cTransformable->posY, cTransformable->posZ));

	//Actualiza la rotacion del objeto de irrlicht
	node->setRotation(core::vector3df(cTransformable->rotX, cTransformable->rotY, cTransformable->rotZ));

	//Actualiza el escalado del objeto de irrlicht
	node->setScale(core::vector3df(cTransformable->scaleX, cTransformable->scaleY, cTransformable->scaleZ));
	
}

//Reajusta la camara 
void RenderFacadeIrrlicht::UpdateCamera(Entity* cam){

	//Cogemos los componentes de la camara
	auto components = cam->GetComponents();
	auto mapTransformable = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransformable->second);

	//Cogemos la posicion de nuestro coche
	//TODO: cambiar ese 0 por el Id del CarManager
	core::vector3df targetPosition  = smgr->getSceneNodeFromId(0)->getPosition();
    targetPosition.Y += 17;
    camera1->setTarget(targetPosition);

	camera1->setPosition(core::vector3df(cTransformable->posX, cTransformable->posY, cTransformable->posZ));

}

//Añade la camara, esto se llama una sola vez al crear el juego
void RenderFacadeIrrlicht::FacadeAddCamera(Entity* goCamera){
	camera1 = smgr->addCameraSceneNode();
	device->getCursorControl()->setVisible(false);

	auto components = goCamera->GetComponents();

	//TODO: Encontrar una mejor manera para acceder a los componentes ya que asi se tarda demasiado
	auto mapTransformable = components.find(CompType::TransformableComp);
	auto cTransformable = static_cast<CTransformable*>(mapTransformable->second);

	auto mapCamera = components.find(CompType::CameraComp);
	auto cCamera = static_cast<CCamera*>(mapCamera->second);

	float posX = cCamera->tarX - 40.0 * sin(((cTransformable->rotX)*PI)/180.0);
	float posZ = cCamera->tarZ - 40.0 * cos(((cTransformable->rotZ)*PI)/180.0);;
	camera1->setTarget(core::vector3df(cCamera->tarX, cCamera->tarY, cCamera->tarZ)); 
	camera1->setPosition(core::vector3df(posX, cTransformable->posY, posZ));
}

bool RenderFacadeIrrlicht::FacadeRun(){
	return device->run();
}

uint32_t RenderFacadeIrrlicht::FacadeGetTime(){
	return device->getTimer()->getTime();
}

void RenderFacadeIrrlicht::FacadeCheckInput(float frameDeltaTime, Entity* car, Entity* cam)
{	
	Data d;
	d.deltaTime  = frameDeltaTime;
	d.gameObject = car;
	d.camera	 = cam;

	if(receiver.IsKeyDown(KEY_KEY_I)){
        eventManager->AddEvent(Event {EventType::PRESS_I,d});
	}else if(receiver.IsKeyDown(KEY_KEY_O)){
        eventManager->AddEvent(Event {EventType::PRESS_O,d});
	}else{
		eventManager->AddEvent(Event {EventType::NO_I_O_PRESS,d});
	}
	
	if(receiver.IsKeyDown(KEY_KEY_D)){
        eventManager->AddEvent(Event {EventType::PRESS_D,d});
	}else if(receiver.IsKeyDown(KEY_KEY_A)){
        eventManager->AddEvent(Event {EventType::PRESS_A,d});
	}else{
		eventManager->AddEvent(Event {EventType::NO_A_D_PRESS,d});
	}
}

int RenderFacadeIrrlicht::FacadeGetFPS(){
	return driver->getFPS();
}

void RenderFacadeIrrlicht::FacadeSetWindowCaption(std::string title){

	//Como transformar de string a wstring (irrlicht)
	std::wstring text_aux;
    for(unsigned int i = 0; i < title.length(); ++i)
    	text_aux += wchar_t( title[i] );

    const wchar_t* txt = text_aux.c_str();

	device->setWindowCaption(txt);
}

//Toda la rutina de limpiar y dibujar de irrlicht
void RenderFacadeIrrlicht::FacadeDraw(){
	driver->beginScene(true, true, video::SColor(255,113,113,133));
	smgr->drawAll(); // draw the 3d scene
	driver->endScene();

}

//Limpia la pantalla
void RenderFacadeIrrlicht::FacadeBeginScene(){
	driver->beginScene(true, true, video::SColor(255,113,113,133));
}

void RenderFacadeIrrlicht::FacadeDrawAll(){
	smgr->drawAll(); // draw the 3d scene
}

void RenderFacadeIrrlicht::FacadeEndScene(){
	driver->endScene();
}

void RenderFacadeIrrlicht::FacadeDeviceDrop(){
	device->drop();
}


RenderFacadeIrrlicht::~RenderFacadeIrrlicht(){
	delete device;
	delete driver;
	delete smgr;
	delete camera1;
}
