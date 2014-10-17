#include "Entity.h"

Entity::Entity(BaseThing& entityBase, int id): Thing(entityBase), id(id) { }

Vec3 Entity::getPosition() {
	return getValueT<Vec3>("$position");
}

Vec3 Entity::getVelocity() {
	return getValueT<Vec3>("$velocity");
}

Vec3 Entity::getAcceleration() {
	return getValueT<Vec3>("$acceleration");
}