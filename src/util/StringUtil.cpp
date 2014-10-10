#include "StringUtil.h"

String Str::capitalize(String str) {
	if (str.size() == 0) return str;
	str[0] = std::toupper(str[0]);
	return str;
}

String Str::simplify(String str) {

	// remove all non alphanumeric characters
	str.erase(remove_if(str.begin(), str.end(), [](char x){return !std::isalnum(x);}), str.end());

	// to lower case
	std::transform(str.begin(), str.end(), str.begin(), tolower);

	return str;
}