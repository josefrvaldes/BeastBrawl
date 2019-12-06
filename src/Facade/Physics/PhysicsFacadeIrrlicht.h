#pragma once

#include <iostream>
#include <memory>
#include "PhysicsFacade.h"
#include "../Render/RenderFacadeManager.h"
#include "../Render/RenderFacadeIrrlicht.h"
#include "../../../lib/irrlicht/irrlicht.h"

using namespace std;
class PhysicsFacadeIrrlicht : public PhysicsFacade{
    public:
        PhysicsFacadeIrrlicht();
        void Update(Entity*, Entity*) override;
        ~PhysicsFacadeIrrlicht();


    private:
        void UpdateCam(Entity*);
        shared_ptr<RenderFacadeIrrlicht> renderEngineIrrlicht;
        scene::ISceneManager* smgr;
};