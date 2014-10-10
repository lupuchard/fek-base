#ifndef GAME_H
#define GAME_H

#include "Renderer.h"
#include "ResourceManager.h"
#include "World.h"

class Game {
public:
	Game();

	void init();
	void execute();
	Happen events();

private:
	void end();

	bool inited;
	std::unique_ptr<WorldSource>     worldSource;
	std::unique_ptr<World>           world;
	std::unique_ptr<Renderer>        renderer;
	std::unique_ptr<ResourceManager> resources;
};

#endif // GAME_H