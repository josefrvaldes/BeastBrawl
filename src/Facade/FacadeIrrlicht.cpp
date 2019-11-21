#include "FacadeIrrlicht.h"




FacadeIrrlicht::FacadeIrrlicht(){
     // ask user for driver
	driverType=driverChoiceConsole();
	device = createDevice(driverType, dimension2d<u32>(640, 480), 16, false, false, false, &receiver);
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();

}

void FacadeIrrlicht::AddSphere(){
	ISceneNode* node = smgr->addSphereSceneNode();
	sphereVector.push_back(node);
}

void FacadeIrrlicht::FacadeInit(){

}

FacadeIrrlicht::~FacadeIrrlicht(){

}