#ifndef WORLDSOURCE_H
#define WORLDSOURCE_H

#include "Item.h"

class WorldSource {
public:

	virtual const PropertySystem& getSystem(ThingType type) = 0;

	/**
	 * Pops an item from the queue of new items. If there are no new items,
	 * this returns null. The item returned will probably be an empty shell
	 * (that is, it will have an id, position and base but no unique values).
	 */
	virtual Item* getNewItem() = 0;

	/**
	 * Fills the given item with it's property values.
	 */
	virtual void fillItemProperties(Item& item) = 0;

};

#endif // WORLDSOURCE_H
