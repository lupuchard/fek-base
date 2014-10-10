#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>
#include <algorithm>

typedef       std::string  String;
typedef const std::string& StringRef;

namespace Str {
	template <class UnaryOperator>
	String trans(String str, UnaryOperator op) {
		std::transform(str.begin(), str.end(), str.begin(), op);
		return str;
	}

	/** Makes the first character of the string capital. */
	String capitalize(String str);

	/**
	 * Removes all text after and including the first period.
	 * Removes all non alphanumeric characters.
	 * Makes the remaining letters lowercase.
	 */
	String simplify(String str);
}

#endif // STRINGUTIL_H
