#include "WorldYamlSource.h"

#include "util/easylogging++.h"
#include <boost/lexical_cast.hpp>

WorldYamlSource::WorldYamlSource(std::string dir): currentItemIndex(0), dir(dir) {
	loadPropertySystems();
	loadItemBases();
	loadItems();
}

void WorldYamlSource::fillItemProperties(Item& item) { }

void WorldYamlSource::loadItemBases() {
	if (!propertySystems[ThingType::ITEM]) return;
	PropertySystem& itemSys = *propertySystems[ThingType::ITEM];

	std::vector<YAML::Node> nodes = YAML::LoadAllFromFile(dir + "ItemBases.yaml");
	for (size_t i = 0; i < nodes.size(); i++) {
		YAML::Node& yaml = nodes[i];

		// create the item
		itemBases.emplace_back(new BaseThing(itemSys));
		BaseThing& item = *itemBases.back();

		// read properties
		for (auto iter = yaml.begin(); iter != yaml.end(); ++iter) {
			String propName = iter->first.as<String>();
			const Property& property = itemSys[propName];
			Variant value = readVariant(iter->second, property.type);
			item.setValue(property, value);
			if (property.mainKey) {
				itemBaseNameMap[boost::lexical_cast<String>(value)] = &item;
			}
		}
	}
}
void WorldYamlSource::loadItems() {
	if (!propertySystems[ThingType::ITEM]) return;
	PropertySystem& itemSys = *propertySystems[ThingType::ITEM];

	std::vector<YAML::Node> nodes = YAML::LoadAllFromFile(dir + "Items.yaml");
	for (size_t i = 0; i < nodes.size(); i++) {
		YamlWrapper yaml(nodes[i]);

		// read base
		String baseName = yaml.read<String>("Base", "", "Item lacks a base.");
		if (baseName == "") continue;
		auto iter = itemBaseNameMap.find(baseName);
		if (iter == itemBaseNameMap.end()) {
			LOG(ERROR) << "'" << baseName << "' is not an existing item base.";
			continue;
		}
		BaseThing& base = *iter->second;

		items.emplace_back(new Item(base, items.size() + 1));
		Item& item = *items.back();

		// read location
		if (yaml["Location"]->IsSequence()) {
			item.moveTo(yaml.read<Coord>("Location", Coord()));
		}

		// read properties
		YAML::Node propertiesNode = yaml["Properties"].getNode();
		for (auto iter = propertiesNode.begin(); iter != propertiesNode.end(); ++iter) {
			const Property& property = itemSys[iter->first.as<String>()];
			item.setValue(property, readVariant(iter->second, property.type));
		}
	}
}
Item* WorldYamlSource::getNewItem() {
	if (currentItemIndex >= items.size()) {
		return nullptr;
	}
	return items[currentItemIndex++].get();
}

void WorldYamlSource::loadPropertySystems() {

	// iterate through each section (each section is a system)
	std::vector<YAML::Node> nodes = YAML::LoadAllFromFile(dir + "PropertySystems.yaml");
	for (size_t i = 0; i < nodes.size(); i++) {
		YamlWrapper yaml(nodes[i]);

		// parse which system this section is for
		String typeName = yaml.read<String>("Which", "NONE", "Invalid property system.");
		ThingType type = ThingType::fromString(typeName);

		// create the system
		propertySystems[type].reset(new PropertySystem());
		PropertySystem& system = *propertySystems[type];

		// parse the properties of the system
		YAML::Node propertiesNode = yaml["Properties"].getNode();
		for (auto iter = propertiesNode.begin(); iter != propertiesNode.end(); ++iter) {
			YamlWrapper propertyYaml(*iter);
			String name  = propertyYaml.read<String>("Name", "", "Property name not given.");
			Type type    = Type::fromString(propertyYaml.read<String>("Type"));
			Variant def  = readVariant(propertyYaml["Default"].getNode(), type);
			bool mainKey = propertyYaml.read<bool>("MainKey", false);
			system.add(name, type, def, mainKey);
		}
	}
}
const PropertySystem& WorldYamlSource::getSystem(ThingType type) {
	return *propertySystems[type];
}

Variant WorldYamlSource::readVariant(YAML::Node& node, Type type) {
	if (!node) return type.getDefault();
	switch(type) {
		case Type::BOOL:   return node.as<bool>();
		case Type::INT:    return node.as<int>();
		case Type::FLOAT:  return node.as<float>();
		case Type::STRING: return node.as<String>();
		default:           return nullptr;
	}
}
