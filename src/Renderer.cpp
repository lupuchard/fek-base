#include "Renderer.h"

#include <OGRE/OgreWindowEventUtilities.h>

Renderer::Renderer(World& world): world(world) {
	root.reset(new Ogre::Root());
	root->restoreConfig();
	window = root->initialise(true, "Great fun happy game.");

	while(true) {
		// Pump window messages for nice behaviour
		Ogre::WindowEventUtilities::messagePump();

		if (window->isClosed()) {
			break;
		}
 
		// Render a frame
		if (!root->renderOneFrame()) break;
	}
}