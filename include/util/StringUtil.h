#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>
#include <algorithm>

typedef       std::string  String;
typedef const std::string& StringRef;

namespace Str {
	template <class UnaryOperator>
	std::string trans(std::string str, UnaryOperator op) {
		std::transform(str.begin(), str.end(), str.begin(), op);
		return str;
	}

	std::string capitalize(const std::string& str);
}

#endif // STRINGUTIL_H
