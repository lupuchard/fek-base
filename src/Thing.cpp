#include "Thing.h"

#include "util/easylogging++.h"

AbstractThing::AbstractThing(const PropertySystem& sys): propertySystem(sys) { }
const PropertySystem& AbstractThing::getSystem() const {
	return propertySystem;
}
Variant AbstractThing::getValue(const std::string& propertyName) const {
	int index = propertySystem.getIndex(propertyName);
	return getValue(index);
}
Variant AbstractThing::getValue(const Property& property) const {
	return getValue(property.index);
}
void AbstractThing::setValue(std::string propertyName, Variant value) {
	setValue(propertySystem.getIndex(propertyName), value);
}
void AbstractThing::setValue(const Property& property, Variant value) {
	setValue(property.index, value);
}



BaseThing::BaseThing(const PropertySystem& sys): AbstractThing(sys) {
	values.resize(getSystem().size());
	values[0] = nullptr;
	for (size_t i = 1; i < getSystem().size(); i++) {
		values[i] = getSystem()[i].def;
	}
}
Variant BaseThing::getValue(size_t propertyIndex) const {
	DCHECK(propertyIndex < values.size());
	return values[propertyIndex];
}
void BaseThing::setValue(size_t propertyIndex, Variant value) {
	DCHECK(propertyIndex < values.size());
	DCHECK(value.which() == getSystem()[propertyIndex].type) << "wrong type";
	values[propertyIndex] = value;
}



Thing::Thing(const AbstractThing& base): AbstractThing(base.getSystem()), base(base) { }
Variant Thing::getValue(size_t propertyIndex) const {
	auto iter = values.find(propertyIndex);
	if (iter == values.end()) {
		return base[propertyIndex];
	}
	return iter->second;
}
void Thing::setValue(size_t propertyIndex, Variant value) {
	DCHECK(value.which() == getSystem()[propertyIndex].type) << "wrong type";
	values[propertyIndex] = value;
}
