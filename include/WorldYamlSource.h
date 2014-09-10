#ifndef WORLDYAMLSOURCE_H
#define WORLDYAMLSOURCE_H

#include "WorldSource.h"
#include "util/YamlWrapper.h"

/**
 * The world source for the server.
 * It simply retrieves the world from a directory
 * of yaml files with the world data.
 */
class WorldYamlSource: public WorldSource {
public:
	WorldYamlSource(String dir);

	void fillItemProperties(Item& item);
	Item* getNewItem();
	const PropertySystem& getSystem(ThingType type);

private:
	void loadPropertySystems();
	void loadItemBases();
	void loadItems();

	Variant readVariant(YAML::Node& node, Type type);

	std::unique_ptr<PropertySystem> propertySystems[ThingType::count()];

	std::vector<std::unique_ptr<BaseThing>> itemBases;
	std::map<String, BaseThing*> itemBaseNameMap;

	std::vector<std::unique_ptr<Item>> items;
	size_t currentItemIndex;

	String dir;
};

#endif // WORLDYAMLSOURCE_H
