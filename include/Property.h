#ifndef PROPERTY_H
#define PROPERTY_H

#include "Type.h"

class Property {
public:
	Property(std::string name, Type type);

	std::string name;
	Type        type;
	Variant     def;
	int         index;
	bool        mainKey;
};

#endif // PROPERTY_H
