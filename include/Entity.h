#ifndef ENTITY_H
#define ENTITY_H

#include "Thing.h"

class Entity: public Thing {
public:
	Entity(BaseThing& entityBase, int id = 0);

	/**
	 * Stat name: $position
	 * @return The current position of the Entity.
	 */
	Vec3 getPosition();

	/**
	 * Stat name: $velocity
	 * @return The current velocity of the Entity.
	 */
	Vec3 getVelocity();

	/**
	 * Stat name: $acceleration
	 * @return The current acceleration of the Entity.
	 */
	Vec3 getAcceleration();


private:
	int id;

};

#endif // ENTITY_H
