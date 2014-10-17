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

void ResourceManager::scanPackages(String directory) {
	if (!filesystem::exists(directory)) return;

	filesystem::directory_iterator endIter;
	for (filesystem::directory_iterator iter(directory); iter != endIter; ++iter) {
		if (filesystem::is_directory(iter->status())) {
			scanPackage(iter->path().string());
		}
	}
}

void ResourceManager::scanPackage(String directory) {
	if (!filesystem::exists(directory)) return;
	filesystem::path path(directory);

	String package = Str::simplify(path.stem().string());

	scanResources(package, directory);

	YamlRes* resDataYaml = peekResource<ResType::YAML>(package + ":" + "extra resource data");
	if (resDataYaml) {
		possiblyLoadResource<YamlRes>(resDataYaml);
		readExtraResourceData(package, resDataYaml->getYaml());
	}
}

void ResourceManager::scanResources(StringRef package, StringRef folder) {
	if (!filesystem::exists(folder)) return;

	filesystem::directory_iterator endIter;
	for (filesystem::directory_iterator iter(folder); iter != endIter; ++iter) {
		auto status = iter->status();
		if (filesystem::is_regular_file(status)) {
			scanResource(package, iter->path().string());
		} else if (filesystem::is_directory(status)) {
			scanResources(package, iter->path().string());
		}
	}
}

// Recursive iteration through the ResTypes so that the correct containers are used.
// Then the resource is constructed and added to the container.
void ResourceManager::add(Resource& res, IntConst<ResType::count()>) { }
template <int resType>
void ResourceManager::add(Resource& res, IntConst<resType>) {
	if (res.type == resType) {

		// construct resource
		static auto thisRes = std::get<resType>(RES_TUPLE_OBJ);
		typedef decltype(thisRes) ThisRes;
		ThisRes* resource = new ThisRes();
		resource->filename = res.filename;
		resource->nameID   = res.nameID;
		resource->package  = res.package;
		
		// add to container
		auto& map  = std::get<resType>(nameIDMaps);
		auto iter = map.find(resource->getNameID());
		if (iter == map.end()) {
			map[resource->getNameID()] = resource;
		} else {
			String rtStr = res.type.toString();
			LOG(ERROR) << rtStr << " resource with nameID '" << res.nameID << "' already exists.";
		}
		
	} else {
		add(res, IntConst<resType + 1>());
	}
}

void ResourceManager::scanResource(StringRef package, StringRef filename) {
	if (!filesystem::exists(filename)) return;
	filesystem::path path(filename);

	Resource res;
	res.type     = extensionMap[path.extension().string()];
	res.filename = filename;
	res.nameID   = package + ':' + Str::simplify(path.stem().string());
	res.package  = package;

	add(res, IntConst<1>());
}

void ResourceManager::readExtraResourceData(StringRef package, const YamlWrapper& yaml) {
	YAML::Node node = *yaml;
	for (auto iter = node.begin(); iter != node.end(); ++iter) {
		String typeName = iter->first.as<String>();
		ResType type = ResType::fromString(typeName);
		switch(type) {
			case ResType::TEX: readExtraTextureData(package, iter->second); break;
			default: break;
		}
	}
}

void ResourceManager::readExtraTextureData(StringRef package, YAML::Node& node) {
	for (auto iter = node.begin(); iter != node.end(); ++iter) {
		String resName = iter->first.as<String>();
		TexRes* tex = peekResource<ResType::TEX>(package + ":" + resName);
		if (!tex) {
			LOG(ERROR) << "Texture '" << resName << "' does not exist. (In ExtraResourceData).";
			continue;
		}
		YamlWrapper yaml(iter->second);
		Coord dim = yaml.read<Coord>("SheetTileDim", Coord(0, 0));
		if (dim != Coord(0, 0)) {
			tex->setSheetTileDim(dim);
		}
	}
}
