#ifndef RENDERER_H
#define RENDERER_H

#include "World.h"

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderWindow.h>

class Renderer {
public:
	Renderer(World& world);

private:
	World& world;

	std::unique_ptr<Ogre::Root> root;
	Ogre::RenderWindow* window;

};

#endif // RENDERER_H