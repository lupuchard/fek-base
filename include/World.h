#ifndef WORLD_H
#define WORLD_H

#include "Item.h"
#include "PropertySystem.h"
#include "WorldSource.h"
#include "util/SmartIter.h"

class World {
public:
	World(WorldSource& source);

	void update(float sec);

	typedef SmartIter<std::vector<Item*>, Item> ItemIter;
	ItemIter exposedItemsBegin();
	ItemIter exposedItemsEnd();
	std::vector<Item*> getExposedItemsCopy() const;

private:
	void retrieveItems();
	
	WorldSource& source;

	PropertySystem itemSystem;
	std::vector<Item*> exposedItems;
	std::vector<Item*> items;

	float secondsPassed;
};

#endif // WORLD_H
