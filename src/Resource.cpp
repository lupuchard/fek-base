#include "Resource.h"

Resource::Resource(ResType type): type(type), sessionID(0), loaded(false) { }

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
StringRef Resource::getPackage() const {
	return package;
}
bool Resource::isLoaded() const {
	return loaded;
}


YamlRes::YamlRes(): Resource(ResType::YAML) { }

void YamlRes::load(irr::IrrlichtDevice* device) {
	yaml.setNode(YAML::LoadFile(getFilename()));
}
const YamlWrapper& YamlRes::getYaml() const {
	return yaml;
}
