#include "util/easylogging++.h"
_INITIALIZE_EASYLOGGINGPP
#undef CHECK

#include "Game.h"

void configureLogging() {
	el::Configurations conf;
	conf.setToDefault();
	conf.setGlobally(el::ConfigurationType::Format, "%msg");
	conf.setGlobally(el::ConfigurationType::Filename, "logs/thelog.log");
	conf.set(el::Level::Warning, el::ConfigurationType::Format, "[Warn] %msg");
	conf.set(el::Level::Error  , el::ConfigurationType::Format, "[ERROR] %msg");
	conf.set(el::Level::Fatal  , el::ConfigurationType::Format, "[FATAL] %msg");
	el::Loggers::reconfigureLogger("default", conf);
	el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

	LOG(INFO) << "---";
}

int main(int argc, char** argv) {
	configureLogging();
	
	Game game;
	game.init();
	game.execute();
	return 0;
}