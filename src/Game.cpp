#include "Game.h"

#include "WorldYamlSource.h"

Game::Game(): inited(false) { }

void Game::init() {
	inited = true;
	worldSource.reset(new WorldYamlSource("tests/World/"));
	world.reset(new World(*worldSource));
	renderer.reset(new Renderer(*world));

	renderer->createWindow();

	resources.reset(new ResourceManager("data/extensions.yaml"));
	resources->scanResources("assets/");
}
void Game::execute() {
	if (!inited) return;

	while(true) {
		Happen end = events();
		if (end == Happen::YES) break;

		end = renderer->update();
		if (end == Happen::YES) break;
	}
	renderer->closeWindow();
}

Happen Game::events() {
	return Happen::NO;
}