#include "RenderFacadeIrrlicht.h"

#include "../Components/CPosition.h"
#include "../Components/CType.h"
#include "../Components/CId.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "../Components/Component.h"
#include "RenderFacadeManager.h"

//PUNTEROS A FUNCIONES
void MoveUp(Data d);


RenderFacadeIrrlicht::RenderFacadeIrrlicht(){
	eventManager = EventManager::GetInstance();

	eventManager.Suscribe(Listener {EventType::PRESS_W,MoveUp});
    // ask user for driver
	device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1280, 720), 16, false, false, false, &receiver);
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();

}

void RenderFacadeIrrlicht::AddReceiver(MyEventReceiver receiver){
	device->setEventReceiver(&receiver);
}



//INPUTS : Una entidad GameObject
//RETURNS: El Id del objeto añadido
//TODO: Llevar cuidado con las rutas de las texturas si luego se mueven las carpetas
uint16_t RenderFacadeIrrlicht::FacadeAddObject(GameObject *go){

	//Fuente: https://stackoverflow.com/questions/11855018/c-inheritance-downcasting
	//Como convertir un Component en cualquier tipo de sus subclases para poder usar los metodos propios
	auto components = go->GetComponents();

	//TODO: Encontrar una mejor manera para acceder a los componentes ya que asi se tarda demasiado
	auto mapTransformable = components.find(CompType::Transformable);
	auto cTransformable = static_cast<CTransformable*>(mapTransformable->second);

	auto mapId = components.find(CompType::Id);
	auto cId = static_cast<CId*>(mapId->second);

	auto mapTexture = components.find(CompType::Texture);
	auto cTexture = static_cast<CTexture*>(mapTexture->second);

	auto mapType = components.find(CompType::Type);
	auto cType 	 = static_cast<CType*>(mapType->second);

	auto mapMesh = components.find(CompType::Mesh);
	auto cMesh	 = static_cast<CMesh*>(mapMesh->second);


	//Switch para añadir el tipo de objeto
	scene::ISceneNode* node;
	std::string meshPath = "../../" + cMesh->GetMesh();

	switch(cType->GetType()){
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

	std::string path = "media/" + cTexture->GetTexture();
	if(node){
		std::cout << "Entra aqui\n";
		node->setID(cId->GetId());
		node->setPosition(core::vector3df(cTransformable->GetPosX(),cTransformable->GetPosY(),cTransformable->GetPosZ()));
		node->setRotation(core::vector3df(cTransformable->GetRotX(),cTransformable->GetRotY(),cTransformable->GetRotZ()));
		node->setScale(core::vector3df(cTransformable->GetScaleX(),cTransformable->GetScaleY(),cTransformable->GetScaleZ()));
		node->setMaterialTexture(0, driver->getTexture(path.c_str())); //Obligado incluir el c_str() si no irrlicht no carga solo con un string
		//auto texture = driver->getTexture("wall.bmp");
		//node->setMaterialTexture(0, texture);

		node->setMaterialFlag(video::EMF_LIGHTING, false);

	}

	sceneObjects[cId->GetId()] = node;

	auto obj = sceneObjects.find(cId->GetId());
	std::cout << "Añadido el objeto con ID: " << obj->first << std::endl;


	return cId->GetId();
}


void RenderFacadeIrrlicht::FacadeAddCamera(){
	smgr->addCameraSceneNodeFPS();
	device->getCursorControl()->setVisible(false);
}

bool RenderFacadeIrrlicht::FacadeRun(){
	return device->run();
}

uint32_t RenderFacadeIrrlicht::FacadeGetTime(){
	return device->getTimer()->getTime();
}

void RenderFacadeIrrlicht::FacadeCheckInput(float frameDeltaTime){
	
	core::vector3df nodePosition = sceneObjects.find(0)->second->getPosition();
	const f32 MOVEMENT_SPEED = 5.f;

	if(receiver.IsKeyDown(irr::KEY_KEY_W))
		nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
	else if(receiver.IsKeyDown(irr::KEY_KEY_S))
		nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

	if(receiver.IsKeyDown(irr::KEY_KEY_A))
		nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
	else if(receiver.IsKeyDown(irr::KEY_KEY_D))
		nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

	if(receiver.IsKeyDown(irr::KEY_ESCAPE)){
		device->closeDevice();
	}
	sceneObjects.find(0)->second->setPosition(nodePosition);
	//node->setPosition(nodePosition);
	
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

}


//PUNTEROS A FUNCIONES

void MoveUp(Data d){
	//TODO: En las funciones delegadas necesitas acceder al RenderManager para que me de los objetos de la escena
	//core::vector3df nodePosition = sceneObjects.find(0)->second->getPosition();
	const f32 MOVEMENT_SPEED = 5.f;

	//nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
	//nodePosition.Y += MOVEMENT_SPEED;

	//sceneObjects.find(0)->second->setPosition(nodePosition);
	//node->setPosition(nodePosition);
}