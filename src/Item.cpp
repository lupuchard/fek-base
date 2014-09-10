#include "Item.h"

#include "util/easylogging++.h"

Item::Item(BaseThing& itemBase, int id): Thing(itemBase), id(id), locationType(NONE) { }

Item::LocationType Item::where() const {
	return locationType;
}

Coord Item::getPosition() const {
	DCHECK(locationType == COORD);
	return pos;
}

void Item::moveTo(Coord pos) {
	locationType = COORD;
	pos = pos;
}
