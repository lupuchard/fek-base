#include "YamlWrapper.h"

#include "easylogging++.h"

YamlWrapper::YamlWrapper(YAML::Node node): node(node) { }

YAML::Node& YamlWrapper::getNode() {
	return node;
}
const YAML::Node& YamlWrapper::getNode() const {
	return node;
}
YAML::Node& YamlWrapper::operator*() {
	return node;
}
const YAML::Node& YamlWrapper::operator*() const {
	return node;
}
YAML::Node* YamlWrapper::operator->() {
	return &node;
}
const YAML::Node* YamlWrapper::operator->() const {
	return &node;
}

YamlWrapper YamlWrapper::operator[](std::string key) const {
	return YamlWrapper(node[key]);
}

void YamlWrapper::printErr(std::string errorMessage) {
	LOG_IF(!errorMessage.empty(), ERROR) << errorMessage;
}
