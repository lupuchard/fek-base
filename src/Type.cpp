#include "Type.h"

Variant Type::getDefault() {
	switch(val) {
		case Type::NONE:   return nullptr;
		case Type::BOOL:   return false;
		case Type::INT:    return 0;
		case Type::FLOAT:  return 0.0f;
		case Type::STRING: return "";
		default: assert(false);
	}
	return nullptr;
}
