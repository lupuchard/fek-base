#ifndef WORLD_H
#define WORLD_H

#include "Item.h"
#include "PropertySystem.h"
#include "WorldSource.h"

class World {
public:
	World(WorldSource& source);

private:
	WorldSource& source;

	PropertySystem itemSystem;
	std::vector<std::unique_ptr<Item>> items;
};

#endif // WORLD_H
