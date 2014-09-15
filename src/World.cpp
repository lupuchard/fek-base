#include "World.h"

#include "util/easylogging++.h"

World::World(WorldSource& source): source(source) {
	retrieveItems();
}

void World::update(float sec) {
	secondsPassed += sec;
	retrieveItems();
}

void World::retrieveItems() {
	Item* item = source.getNewItem();
	while(item) {
		items.push_back(item);
		if (item->where() == Item::COORD) {
			exposedItems.push_back(item);
		}
		item = source.getNewItem();
	}
}

World::ItemIter World::exposedItemsBegin() {
	return ItemIter::begin(exposedItems);
}
World::ItemIter World::exposedItemsEnd() {
	return ItemIter::end(exposedItems);
}
std::vector<Item*> World::getExposedItemsCopy() const {
	return exposedItems;
}

/*size_t World::getNumExposedItems() const {
	return exposedItems.size();
}
const Item& World::getExposedItem(size_t index) const {
	DCHECK(index < exposedItems.size());
	return *exposedItems[index];
}
Item& World::getExposedItem(size_t index) {
	DCHECK(index < exposedItems.size());
	return *exposedItems[index];
}*/