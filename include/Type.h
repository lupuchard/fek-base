#ifndef TYPE_H
#define TYPE_H

#include "util/Enum.h"
#include "util/Vec.h"
#include <boost/variant.hpp>

/*
 * The various types that properties can have.
 * NONE is worthless.
 * BOOL is a c++ bool,
 *		true: 0, "true", or "yes"
 *		false: 0, "false", or "no"
 * INT is a c++ int. Floats passed will be rounded down.
 * FLOAT is a c++ float.
 * STRING is a c++ std::string.
 * COORD is a 2d integral vector.
 *		In yaml: [x, y]
 *		In lua: {x=, y=} table or {x, y} array
 * VEC is a 3d floating point vector
 *		In yaml: [x, y, z]
 *		In lua: {x=, y=, z=} table or {x, y, z} array
 * MESH refers to either a mesh resource or a special method of rendering
 *		As resource: just the nameID of the resource
 *		Otherwise: either NONE or BILLBOARD
 */

typedef boost::variant<void*, bool, int, float, std::string, Coord, Vec3> Variant;

typedef     std::tuple<void*, bool, int, float, std::string, Coord, Vec3> TypeTuple;
ENUM_BEGIN(Type, NONE, BOOL, INT, FLOAT, STRING, COORD, VEC, MESH);

/** @return The default value a variable of the given type should have. */
Variant getDefault();

ENUM_END


#endif // TYPE_H
