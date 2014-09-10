#include "PropertySystem.h"

#include "util/easylogging++.h"

PropertySystem::PropertySystem(): mainKey(nullptr) {
	properties.push_back(nullptr);
}

const Property& PropertySystem::add(StringRef name, Type type, bool isMainKey) {
	if (nameToIndexMap.count(name)) {
		LOG(ERROR) << "A property named '" << name << "' already exists.";
		return *properties[0];
	}

	properties.emplace_back(new Property(name, type));
	Property& property = *properties.back();
	property.index = properties.size() - 1;
	nameToIndexMap[property.name] = property.index;

	if (isMainKey) {
		if (mainKey) {
			LOG(ERROR) << "A main key already exists in this system.";
		} else {
			mainKey = &property;
			property.mainKey = true;
		}
	}

	return property;
}
const Property& PropertySystem::add(StringRef name, Type type, Variant def, bool isMainKey) {
	const Property& prop = add(name, type, isMainKey);
	properties.back()->def = def;
	return prop;
}

size_t PropertySystem::size() const {
	return properties.size();
}

const Property& PropertySystem::at(size_t index) const {
	DCHECK(index > 0 && index < properties.size()) << "index out of bounds";
	return *properties[index];
}

const Property& PropertySystem::withName(const std::string& name) const {
	return *properties[getIndex(name)];
}

size_t PropertySystem::getIndex(const std::string& name) const {
	auto iter = nameToIndexMap.find(name);
	if (iter == nameToIndexMap.end()) {
		LOG(ERROR) << "There is no Property with the name '" << name << "'.";
		return 0;
	}
	return iter->second;
}

const Property& PropertySystem::getMainKey() const {
	DCHECK(mainKey);
	return *mainKey;
}
