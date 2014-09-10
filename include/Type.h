#ifndef TYPE_H
#define TYPE_H

#include "util/Enum.h"
#include <boost/variant.hpp>

typedef boost::variant<void*, bool, int, float, std::string> Variant;

ENUM_BEGIN(Type, NONE, BOOL, INT, FLOAT, STRING);

/**
 * @return The default value a variable of the given type should have.
 */
Variant getDefault();

ENUM_END


#endif // TYPE_H
