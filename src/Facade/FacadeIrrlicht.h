#ifndef __FACADEIRRLICHT_H__
#define __FACADEIRRLICHT_H__

#include <iostream>
#include "Facade.h"
#include <irrlicht.h>
#include "driverChoice.h"
#include <vector>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;


class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	
	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

class FacadeIrrlicht : public Facade {
    public:
        FacadeIrrlicht();
        void FacadeInit() override;
        void AddSphere();
        ~FacadeIrrlicht();

    private:
        E_DRIVER_TYPE driverType;
        IrrlichtDevice* device;
        IVideoDriver* driver;
        ISceneManager* smgr;
        MyEventReceiver receiver;

        vector<ISceneNode*> sphereVector;
};


#endif 