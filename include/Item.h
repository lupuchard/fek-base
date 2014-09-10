#ifndef ITEM_H
#define ITEM_H

#include "Thing.h"
#include "util/Vec.h"

typedef int Time;

class Item: public Thing {
public:
	Item(BaseThing& itemBase, int id = 0);


	enum LocationType {NONE, COORD, CONTAINER};

	/** @return The type of location this Item has. */
	LocationType where() const;

	/**
	 * @pre where() == Item::COORD
	 * @return Where the Item is in the world.
	 */
	Coord getPosition() const;

	/** Changes the Item's location type to COORD and sets it to the given position. */
	void moveTo(Coord pos);

private:
	int id;
	Time lastUpdated;

	union {
		void* container;
		Coord pos;
	};
	LocationType locationType;
};

#endif // ITEM_H
