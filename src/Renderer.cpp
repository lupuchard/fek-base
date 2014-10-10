#include "Renderer.h"

using namespace irr;
using namespace core;
using namespace video;

Renderer::Renderer(World& world): world(world),
	device(nullptr), driver(nullptr), scenes(nullptr), guiEnv(nullptr) { }

Result Renderer::createWindow() {
	dimension2d<u32> dim(640, 480);
	device = createDevice(video::EDT_OPENGL, dim, 32, false, false, false, 0);

	if (!device) {
		return Result::FAILURE;
	}

	device->setWindowCaption(L"Howdy");

	driver = device->getVideoDriver();
	scenes = device->getSceneManager();
	guiEnv = device->getGUIEnvironment();

	guiEnv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!", rect<s32>(10,10,260,22), true);

	return Result::SUCCESS;
}

void Renderer::closeWindow() {
	driver->drop();
}

Happen Renderer::update() {
	bool success = device->run();
	if (!success) return Happen::YES;

	driver->beginScene(true, true, SColor(255, 100, 101, 140));

	scenes->drawAll();
	guiEnv->drawAll();

	driver->endScene();

	return Happen::NO;
}

irr::IrrlichtDevice* Renderer::getDevice() {
	return device;
}