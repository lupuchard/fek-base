#ifndef YAMLWRAPPER_H
#define YAMLWRAPPER_H

#include "Vec.h"
#include "StringUtil.h"
#include "yaml-cpp/yaml.h"

class YamlWrapper {
	public:
		YamlWrapper();
		YamlWrapper(YAML::Node);
		void setNode(YAML::Node);

		/** Returns this asset's yaml node. @{ */
		YAML::Node& getNode();
		const YAML::Node& getNode() const;
		YAML::Node& operator*();
		const YAML::Node& operator*() const;
		YAML::Node* operator->();
		const YAML::Node* operator->() const;
		/// @}

		/** Go within. */
		YamlWrapper operator[](std::string key) const;

		/**
		 * Attempts to retrieve the given value from this yaml asset.
		 * @param key The key to the value you want.
		 * @param def The default value to be returned if the given key does not exist.
		 * @param errorMessage The message to be printed if the given key does not exist. Defaults to printing nothing.
		 */
		template <typename T>
		T read(std::string key, T def = T(), std::string errorMessage = "") const {
			YAML::Node n = node[key];
			if (n) return n.as<T>();
			printErr(errorMessage);
			return def;
		}

		/**
		 * Attempts to retrieve the given list from this yaml asset as a vector.
		 * @param key The key to the list you want.
		 * @param errorMessage The message to be printed if the given key does not exist. Defaults to printing nothing.
		 * @
		 */
		template <typename T>
		std::vector<T> readList(std::string errorMessage = "") const {
			std::vector<T> vect;
			if (node.IsSequence()) {
				for (auto iter = node.begin(); iter != node.end(); ++iter) {
					vect.push_back(iter->as<T>());
				}
			} else {
				printErr(errorMessage);
			}
			return vect;
		}

		/**
		 * Attempts to retrieve the given map from this yaml asset as a vector.
		 * @param key The key to the map you want.
		 * @param errorMessage The message to be printed if the given key does not exist. Defaults to printing nothing.
		 */
		template <typename K, typename V>
		std::map<K, V> readMap(std::string errorMessage = "") const {
			std::map<K, V> map;
			if (node.IsMap()) {
				for (auto iter = node.begin(); iter != node.end(); ++iter) {
					map.insert(std::pair<K, V>(iter->first.as<K>(), iter->second.as<V>()));
                }
			} else {
				printErr(errorMessage);
			}
			return map;
		}

	private:
		static void printErr(std::string errorMessage);

		YAML::Node node;
};

/** A specialization for bool values that will return boolean values for "yes", "no", "true" and "false" */
template<> inline bool YamlWrapper::read<bool>(std::string key, bool def, std::string errorMessage) const {
	YAML::Node n = node[key];
	if (n) {
		std::string str = Str::trans(n.as<std::string>(), ::tolower);
		if     (str == "yes" || str == "true")  return true;
		else if (str == "no" || str == "false") return false;
		else return read<int>(key, def, errorMessage);
	} else {
		printErr(errorMessage);
		return def;
	}
}

// Some custom data type encoding and decoding
namespace YAML {
	template<typename T> struct convert<Vector<T>> {
		static Node encode(const Vector<T>& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}
		static bool decode(const Node& node, Vector<T>& rhs) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}
			rhs.x = node[0].as<T>();
			rhs.y = node[1].as<T>();
			return true;
		}
	};
	template<typename T> struct convert<Vector3<T>> {
		static Node encode(const Vector3<T>& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, Vector3<T>& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}
			rhs.x = node[0].as<T>();
			rhs.y = node[1].as<T>();
			rhs.z = node[2].as<T>();
			return true;
		}
	};
}

#endif // YAMLWRAPPER_H
