#ifndef PROPERTYSYSTEM_H
#define PROPERTYSYSTEM_H

#include "Property.h"
#include "util/StringUtil.h"

#include <memory>
#include <vector>

/**
 * A PropertySystem is a list of properties.
 */
class PropertySystem {
public:
	PropertySystem();

	/** Adds a property to the system. */
	const Property& add(StringRef name, Type type, bool isMainKey = false);
	const Property& add(StringRef name, Type type, Variant def, bool isMainKey = false);

	/** @return The number of properties, including the null property at index 0. */
	size_t size() const;

	/** @return The property with the given index. */
	const Property& at(size_t index) const;
	/** @return at(index) */
	inline const Property& operator[](size_t index) const {
		return at(index);
	}

	/** @return The property with the given name. */
	const Property& withName(StringRef name) const;
	/** @return withName(name) */
	inline const Property& operator[](StringRef name) const {
		return withName(name);
	}

	/**
	 * @return The index of the property with the given name,
	 * or 0 if there is no property with that name.
	 */
	size_t getIndex(const std::string& name) const;

	/**
	 * The main key is the property used to differentiate
	 * different BaseThings. Each should have a unique
	 * value for their main key.
	 */
	const Property& getMainKey() const;

private:
	std::vector<std::unique_ptr<Property>> properties;
	std::unordered_map<std::string, size_t> nameToIndexMap;
	Property* mainKey;
};

#endif // PROPERTYSYSTEM_H
