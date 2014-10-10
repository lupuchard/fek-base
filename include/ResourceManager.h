#ifndef RES_MANAGER_H
#define RES_MANAGER_H

#include "Resource.h"
#include "IrrResource.h"
#include "util/TemplateUtil.h"
#include <easylogging++.h>

typedef std::tuple<Resource, YamlRes, TexRes> ResTuple;
const ResTuple RES_TUPLE_OBJ;

class ResourceManager {
	public:
		ResourceManager(String extensionsFile);
		void setDevice(irr::IrrlichtDevice* device);

		/** Scans a directory recursively for resources. */
		void scanResources(String directory);

		/**
		 * Doesn't load the resource, instead it simply stores it's filenames
		 * and assigns it a name ID and session ID so that you can get it
		 * later with getResource or peekResource.
		 */
		void scanResource(String filename);


		/**
		 * Returns the resource with the given name ID.
		 * Resource data will be loaded if it has not been.
		 * Returns null if no resource with the given name is found.
		 */
		template<ResType::Enum type>
		auto* getResource(String name) {
			std::string str = Str::simplify(name);
			auto& map = std::get<type>(resourceMaps);
			auto iter = map.find(str);
			if (iter != map.end()) {
				return possiblyLoadResource(iter->second);
			}
			LOG(ERROR) << ResType(type).toString() << " resource '" << str << "' not found.";
			return (decltype(iter->second))nullptr; // haha i had to cast nullptr to a pointer
		}

		/**
		 * Returns the resource with the given name ID.
		 * Unlike getResource(), this one will not load unloaded resource data.
		 * Also, no error is printed when a resource is not found. But null is returned.
		 */
		template<ResType::Enum type>
		auto peekResource(String name) const {
			auto& map = std::get<type>(resourceMaps);
			auto iter = map.find(Str::simplify(name));
			if (iter != map.end()) return iter->second;
			return (decltype(iter->second))nullptr;
		}

		/**
		 * Returns the resource with the given session ID.
		 * Resource data will be loaded if it has not been.
		 * Returns null if no resource with the given session ID is found.
		 */
		template<ResType::Enum type>
		auto* getResource(size_t index) {
			auto& vect = std::get<type>(resources);
			if (index < vect.size()) {
				return possiblyLoadResource(vect[index]);
			}
			LOG(ERROR) << ResType(type).toString() << " resource " << index << " not found.";
			return (decltype(vect[index]))nullptr;
		}

		/**
		 * Returns the resource with the given session ID.
		 * Unlike getResource(), this one will not load unloaded resource data.
		 * Also, no error is printed when a resource is not found. But null is returned.
		 */
		template<ResType::Enum type>
		const auto* peekResource(size_t index) const {
			auto& vect = std::get<type>(resources);
			if (index < vect.size()) return vect[index];
			return (decltype(vect[index]))nullptr;
		}

	private:
		template<class R>
		R* possiblyLoadResource(R* res) {
			if (!res->isLoaded()) {
				res->load(device);
			}
			return res;
		}

		void add(ResType type, StringRef filename, StringRef nameID, IntConst<ResType::count()>);
		template<int resType>
		void add(ResType type, StringRef filename, StringRef nameID, IntConst<resType>);

		std::map<String, ResType> extensionMap;
		irr::IrrlichtDevice* device;

		ContainerTupleT<std::vector, ContainerTupleT<std::unique_ptr, ResTuple>> resources;

		template <class T> using StrMap = std::unordered_map<String, T>;
		ContainerTupleT<StrMap, PointerTupleT<ResTuple>> resourceMaps;
};

#endif // RES_MANAGER_H
