#include "Game.h"

#include "WorldYamlSource.h"

Game::Game(): inited(false) { }

void Game::init() {
	inited = true;
	worldSource.reset(new WorldYamlSource("tests/World/"));
	world.reset(new World(*worldSource));
	renderer.reset(new Renderer(*world));

	for (auto iter = world->exposedItemsBegin(); iter != world->exposedItemsEnd(); ++iter) {
		std::cout << iter->getValueT<String>("name") << std::endl;
	}
}
void Game::execute() {
	if (!inited) return;

	while(true) {
		Happen end = events();
		if (end == Happen::YES) {
			return;
		}


	}
}

Happen Game::events() {
	return Happen::YES;
}