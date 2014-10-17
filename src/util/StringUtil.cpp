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

String Str::simplify(String str, char keep) {

	// remove all non alphanumeric or kept characters
	str.erase(remove_if(str.begin(), str.end(), [keep](char x) {
		return !std::isalnum(x) && x != keep;
	}), str.end());

	// to lower case
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}

String Str::simplify(String str, String keep) {

	// remove all non alphanumeric or kept characters
	str.erase(remove_if(str.begin(), str.end(), [keep](char x) {
		size_t pos = keep.find(x);
		return !std::isalnum(x) && pos == std::string::npos;
	}), str.end());

	// to lower case
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}
