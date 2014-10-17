#ifndef RESOURCE_H
#define RESOURCE_H

#include "util/Enum.h"
#include "util/YamlWrapper.h"

/** The different types of resources. */
ENUM(ResType, NONE, YAML, TEX);

// Forward declare IrrlichtDevice
namespace irr {
	class IrrlichtDevice;
}

class Resource {
public:
	Resource(ResType type = ResType::NONE);

	/** Parses the resource data. */
	virtual void load(irr::IrrlichtDevice* device = nullptr) {}

	/** The type of resource. */
	ResType getType() const;

	/** @return The file this resource is found in. */
	StringRef getFilename() const;

	/** @return The unique simplified name you may refer to this resource with. */
	StringRef getNameID() const;

	/** @return The package this resource is a part of. */
	StringRef getPackage() const;

	/**
	 * The id this resource has for this session.
	 * May not be consistant across sessions. The simplified
	 * name should be used for saving.
	 */
	size_t getSessionID() const;

	/** Whether or not this resource has been loaded into memory yet. */
	bool isLoaded() const;

private:
	friend class ResourceManager;
	ResType type;
	std::string filename;
	std::string nameID;
	std::string package;
	size_t sessionID;
	bool loaded = false;
};


/** A yaml file. */
class YamlRes: public Resource {
public:
	YamlRes();
	void load(irr::IrrlichtDevice* device = nullptr) override;
	const YamlWrapper& getYaml() const;
private:
	YamlWrapper yaml;
};

#endif // RESOURCE_H
