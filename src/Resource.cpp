#include "Resource.h"

Resource::Resource(): type(ResType::NONE), sessionID(0), loaded(false) { }
Resource::Resource(String filename, String nameID, size_t sessionID): type(ResType::NONE),
		filename(filename), nameID(nameID), sessionID(sessionID) { }
Resource::Resource(ResType type, String filename, String nameID, size_t sessionID): type(type),
		filename(filename), nameID(nameID), sessionID(sessionID) { }

ResType Resource::getType() const {
	return type;
}
size_t Resource::getSessionID() const {
	return sessionID;
}
StringRef Resource::getFilename() const {
	return filename;
}
StringRef Resource::getNameID() const {
	return nameID;
}
bool Resource::isLoaded() const {
	return loaded;
}


YamlRes::YamlRes(): Resource() { }
YamlRes::YamlRes(String filename, String nameID, size_t sessionID):
		Resource(ResType::YAML, filename, nameID, sessionID) { }

void YamlRes::load(irr::IrrlichtDevice* device) {
	yaml.setNode(YAML::LoadFile(getFilename()));
}
const YamlWrapper& YamlRes::getYaml() const {
	return yaml;
}
