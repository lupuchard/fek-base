#include "util/easylogging++.h"
_INITIALIZE_EASYLOGGINGPP
#undef CHECK

#include "Game.h"

int main(int argc, char** argv) {
	Game game;
	game.init();
	game.execute();
	return 0;
}