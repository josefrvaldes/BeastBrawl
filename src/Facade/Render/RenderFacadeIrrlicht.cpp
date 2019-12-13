#include "RenderFacadeIrrlicht.h"

#include <math.h>
#include "../../Aliases.h"
#include "../../Components/CCamera.h"
#include "../../Components/CId.h"
#include "../../Components/CMesh.h"
#include "../../Components/CTexture.h"
#include "../../Components/CTransformable.h"
#include "../../Components/CType.h"
#include "../../Components/Component.h"
#include "../../Components/CWayPoint.h"
#include "../../Components/CWayPointEdges.h"
#include "../../Components/CDimensions.h"
#include "../../Entities/WayPoint.h"
#include <math.h>

#define PI 3.14159

//PUNTEROS A FUNCIONES
RenderFacadeIrrlicht::~RenderFacadeIrrlicht() {
}

RenderFacadeIrrlicht::RenderFacadeIrrlicht() {
    //auto inputFacade = InputFacadeManager::GetInstance()->GetInputFacade();
    //auto inputFacadeIrrlicht = static_cast<InputFacadeIrrlicht*>(inputFacade);
    device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1280, 720), 16, false, false, false, &receiver);
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
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
	float width  = (edges[5].X - edges[1].X) * cTransformable->scale.x;
	//cout << "Altura-Anchura-Profundidad: " << width << "-" << height << "-"<< depth << endl;
	float depth  = (edges[2].Z - edges[0].Z) * cTransformable->scale.z;
	shared_ptr<CDimensions> cDimensions = make_shared<CDimensions>(width,height,depth);
	entity->AddComponent(cDimensions); //Le añadimos el componente CDimensions al Entity que sea

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
}

//Reajusta la camara
void RenderFacadeIrrlicht::UpdateCamera(Entity* cam) {
    //Cogemos los componentes de la camara
    auto cTransformable = static_cast<CTransformable*>(cam->GetComponent(CompType::TransformableComp).get());

    //Cogemos la posicion de nuestro coche
    //TODO: cambiar ese 0 por el Id del CarManager
    core::vector3df targetPosition = smgr->getSceneNodeFromId(idCar)->getPosition();
    targetPosition.Y += 17;
    camera1->setTarget(targetPosition);

    camera1->setPosition(core::vector3df(cTransformable->position.x, cTransformable->position.y, cTransformable->position.z));
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
}

bool RenderFacadeIrrlicht::FacadeRun() {
    return device->run();
}

uint32_t RenderFacadeIrrlicht::FacadeGetTime() {
    return device->getTimer()->getTime();
}

void RenderFacadeIrrlicht::FacadeCheckInput() {
    shared_ptr<EventManager> eventManager = EventManager::GetInstance();

	
    if (receiver.IsKeyDown(KEY_ESCAPE)) {
        device->closeDevice();
    }
    if (receiver.IsKeyDown(KEY_KEY_1)) {
        eventManager->AddEventMulti(Event{EventType::PRESS_1});
    }
    if (receiver.IsKeyDown(KEY_KEY_I)) {
        //cout << "Pulsamos I" << endl;
        DataMap data;
        // data["int"] = 123;
        // data["float"] = 2.4f;
        // vector<int> vec;
        // vec.push_back(1);
        // vec.push_back(2);
        // vec.push_back(3);
        // data["vector"] = vec;
        // cout << "Creamos el data" << endl;
        // cout << "Se pulsa I y enviamos el data" << endl;
        eventManager->AddEventMulti(Event{EventType::PRESS_I, data});
    } else if (receiver.IsKeyDown(KEY_KEY_O)) {
        eventManager->AddEventMulti(Event{EventType::PRESS_O});
    } else {
        eventManager->AddEventMulti(Event{EventType::NO_I_O_PRESS});
    }

    if (receiver.IsKeyDown(KEY_KEY_D)) {
        eventManager->AddEventMulti(Event{EventType::PRESS_D});
    } else if (receiver.IsKeyDown(KEY_KEY_A)) {
        eventManager->AddEventMulti(Event{EventType::PRESS_A});
    } else {
        eventManager->AddEventMulti(Event{EventType::NO_A_D_PRESS});
    }
	if(receiver.IsKeyDown(KEY_F3)){
		showDebug = !showDebug;
	}



    // POWERUPS
    if (receiver.IsKeyDown(KEY_KEY_C)) {
        eventManager->AddEventMulti(Event{EventType::PRESS_C});
    }
    if (receiver.IsKeyDown(KEY_SPACE)) {
        eventManager->AddEventMulti(Event{EventType::PRESS_SPACE});
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
void RenderFacadeIrrlicht::FacadeDrawGraphEdges(ManWayPoint* manWayPoints){
	if(!showDebug) return; //Si no esta activado debug retornamos

	//Recorremos todos los WayPoints del manager
	for(shared_ptr<WayPoint> way : manWayPoints->GetEntities()){

        auto cWayPoint = static_cast<CWayPoint*>(way->GetComponent(CompType::WayPointComp).get());
        auto cWayPointEdge = static_cast<CWayPointEdges*>(way->GetComponent(CompType::WayPointEdgesComp).get());

		//Recorremos el componente CWayPointEdges->edges para ir arista a arista
        for(Edge e : cWayPointEdge->edges){
			//Cogemos la posicion de la arista que apunta e->to
        	auto cWayPoint2 = static_cast<CWayPoint*>(manWayPoints->GetEntities()[e.to]->GetComponent(CompType::WayPointComp).get());
			

			video::SMaterial m;
			m.Lighting=false;
			driver->setMaterial(m);
			driver->setTransform(video::ETS_WORLD, core::matrix4());
			core::vector3df initial = core::vector3df(cWayPoint->position.x,cWayPoint->position.y,cWayPoint->position.z);
			core::vector3df final = core::vector3df(cWayPoint2->position.x,cWayPoint2->position.y,cWayPoint2->position.z);

			//Usamos un color u otro en funcion de la distancia
			if(e.cost<300){
				
				driver->draw3DLine(initial,final,video::SColor(255,0,0,255));
			}else if(e.cost>=300 && e.cost<500){
				driver->draw3DLine(initial,final,video::SColor(255,0,255,0));

			}else if(e.cost>=500){
				driver->draw3DLine(initial,final,video::SColor(255,255,0,0));

			}
        }


    }
}

void RenderFacadeIrrlicht::DeleteEntity(Entity* entity) {
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    scene::ISceneNode* node = smgr->getSceneNodeFromId(cId->id);
    node->remove();
}

void RenderFacadeIrrlicht::FacadeDrawBoundingBox(Entity* entity, bool colliding){
	if(!showDebug) return; //Si no esta activado debug retornamos

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
	m.Lighting=false;
	driver->setMaterial(m);
	driver->setTransform(video::ETS_WORLD, core::matrix4());
	if(colliding){

		driver->draw3DBox(boundingBox,video::SColor(255,255,0,0));
	}else{
		driver->draw3DBox(boundingBox,video::SColor(255,0,255,0));

	}
}

