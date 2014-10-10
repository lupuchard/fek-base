#ifndef RENDERER_H
#define RENDERER_H

#include "World.h"
#include <irrlicht/irrlicht.h>

class Renderer {
public:
	Renderer(World& world);
	Result createWindow();
	Happen update();
	void   closeWindow();

	irr::IrrlichtDevice* getDevice();

private:
	World& world;

	irr::IrrlichtDevice*       device;
	irr::video::IVideoDriver*  driver;
	irr::scene::ISceneManager* scenes;
	irr::gui::IGUIEnvironment* guiEnv;
};

#endif // RENDERER_H
