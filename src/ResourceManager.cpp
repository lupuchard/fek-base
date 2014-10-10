#include "ResourceManager.h"

#include "util/YamlWrapper.h"
#include <boost/filesystem.hpp>
using namespace boost;

ResourceManager::ResourceManager(std::string extensionsFile): device(nullptr) {
	YamlWrapper yaml(YAML::LoadFile(extensionsFile));
	for (int i = 1; i < ResType::count(); i++) {
		std::vector<String> strs = yaml[ResType(i).toString()].readList<String>();
		for (size_t j = 0; j < strs.size(); j++) {
			extensionMap["." + strs[j]] = i;
		}
	}
}

void ResourceManager::setDevice(irr::IrrlichtDevice* newDevice) {
	device = newDevice;
}

void ResourceManager::scanResources(std::string folder) {
	if (!filesystem::exists(folder)) return;

	filesystem::directory_iterator endIter; // default construction yields past-the-end
	for (filesystem::directory_iterator iter(folder); iter != endIter; ++iter) {
		auto status = iter->status();
		if (filesystem::is_regular_file(status)) {
			scanResource(iter->path().string());
		} else if (filesystem::is_directory(status)) {
			scanResources(iter->path().string());
		}
	}
}

// Recursive iteration through the ResTypes so that the correct containers are used.
// Then the resource is constructed and added to the container.
void ResourceManager::add(ResType, StringRef, StringRef, IntConst<ResType::count()>) { }
template <int resType>
void ResourceManager::add(ResType type, StringRef filename, StringRef nameID, IntConst<resType>) {
	if (type == resType) {

		// get containers
		auto& vect = std::get<resType>(resources);
		auto& map  = std::get<resType>(resourceMaps);

		// construct resource
		static auto thisRes = std::get<resType>(RES_TUPLE_OBJ);
		typedef decltype(thisRes) ThisRes;
		ThisRes* resource = new ThisRes(filename, nameID, vect.size());
		
		// add to containers
		vect.emplace_back(resource);
		map[resource->getNameID()] = resource;
		
	} else {
		add(type, filename, nameID, IntConst<resType + 1>());
	}
}

void ResourceManager::scanResource(std::string filename) {
	filesystem::path p(filename);

	String nameID    = Str::simplify(p.stem().string());
	String extension = p.extension().string();
	ResType type = extensionMap[extension];

	add(type, filename, nameID, IntConst<1>());
}
