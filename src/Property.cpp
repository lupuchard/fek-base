#include "Property.h"

Property::Property(std::string name, Type type): name(name), type(type), index(0), mainKey(false) {
	def = type.getDefault();
}
