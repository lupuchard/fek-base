#ifndef STRINGMATCHER_H
#define STRINGMATCHER_H

#include <map>
#include <string>
#include <cstddef>

template<typename I>
class StringMatcher {
	public:
		/**
		 * @param strings An array of strings of length len to match with the integers.
		 * @param values The values matched with the strings. Must have the same length as the strings.
		 * @param defaultValue When a string is matched with no value, this is returned instead.
		 */
		template <size_t N>
		StringMatcher(const std::string (&strings)[N], const I (&values)[N], I defaultValue = (I)0) {
			def = defaultValue;
			for (size_t i = 0; i < N; i++) {
				strMap[strings[i]] = values[i];
			}
		}

		/**
		 * This constructor, unlike the other one, automatically matches each string
		 * in the array to it's index.
		 * @param strings An array of strings of length len to match with the integers.
		 * @param defaultValue When a string is matched with no value, this is returned instead.
		 */
		template <size_t N>
		StringMatcher(const std::string (&strings)[N], I defaultValue = (I)0) {
			def = defaultValue;
			for (size_t i = 0; i < N; i++) {
				strMap[strings[i]] = (I)i;
			}
		}

		/**
		 * @param str The string to match.
		 * @return The value matched to the string.
		 */
		I match(std::string str) const {
			auto iter = strMap.find(str);
			if (iter == strMap.end()) return def;
			return iter->second;
		}

	private:
		std::map<std::string, I> strMap;
		I def;
};

#endif // STRINGMATCHER_H
