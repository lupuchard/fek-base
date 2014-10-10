#ifndef THING_H
#define THING_H

#include "PropertySystem.h"
#include <unordered_map>

ENUM(ThingType, NONE, ITEM);

/**
 * The base class for the two things.
 * A thing basically just has values for the different properties.
 * You must provide a property system so it can
 * know what to have values for.
 */
class AbstractThing {
public:
	AbstractThing(const PropertySystem& sys);

	const PropertySystem& getSystem() const;

	/**
	 * @return The value for the given property.
	 * @{ */
	virtual Variant  getValue(size_t propertyIndex) const = 0;
	inline Variant operator[](size_t propertyIndex) const {
		return getValue(propertyIndex);
	}
	       Variant   getValue(const std::string& propertyName) const;
	inline Variant operator[](const std::string& propertyName) const {
		return getValue(propertyName);
	}
	       Variant   getValue(const Property& property) const;
	inline Variant operator[](const Property& property) const {
		return getValue(property);
	}
	template<typename V, typename K>
	V getValueT(K key) const {return boost::get<V>(getValue(key));}
	/// @}

	/**
	 * Sets the given property to the given value.
	 * @{ */
	virtual void setValue(size_t propertyIndex, Variant value) = 0;
	void setValue(std::string propertyName, Variant value);
	void setValue(const Property& property, Variant value);
	/// @}

private:
	const PropertySystem& propertySystem;
};

/**
 * The BaseThing stores the property values
 * in an array long enough to hold all the values,
 * so it has efficient retrieval.
 */
class BaseThing: public AbstractThing {
public:
	BaseThing(const PropertySystem& sys);

	Variant getValue(size_t propertyIndex) const override;
	using AbstractThing::getValue;

	void setValue(size_t propertyIndex, Variant value) override;
	using AbstractThing::setValue;

private:
	std::vector<Variant> values;
};

/**
 * The Thing stores the property values in a hash map
 * so it does not need to have a value for every property.
 * It also has a base thing and if a property value is requested
 * that is not in the map then it will get it from the base instead.
 */
class Thing: public AbstractThing {
public:
	Thing(const AbstractThing& base);

	Variant getValue(size_t propertyIndex) const override;
	using AbstractThing::getValue;

	void setValue(size_t propertyIndex, Variant value) override;
	using AbstractThing::setValue;

private:
	const AbstractThing& base;

	std::unordered_map<size_t, Variant> values;
};

#endif // THING_H
