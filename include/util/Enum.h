#ifndef ENUM_H
#define ENUM_H

#include "StringMatcher.h"
#include <boost/preprocessor.hpp>

/**
 * The best enums.
 * These enums are so great you don't even know.
 */
#define ENUM(ENUM_TYPE, ...) ENUM_BEGIN(ENUM_TYPE, __VA_ARGS__) ENUM_END

#define ENUM_BEGIN(ENUM_TYPE, ...) struct ENUM_TYPE { \
	enum Enum {__VA_ARGS__}; \
	ENUM_STRUCT(ENUM_TYPE, BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) ) \

#define ENUM_END };

#define ENUM_STRUCT(ENUM_TYPE, VALUES) \
	static constexpr int count() {return BOOST_PP_LIST_SIZE(VALUES);} \
	static const std::string& toString(ENUM_TYPE type) { \
		static const std::string STRARR[] = {ENUM_STRFILL(VALUES)}; \
		static const std::string STREMPTY = std::string(); \
		if ((int)type.val < 0 || (int)type.val >= count()) return STREMPTY; \
		return STRARR[type.val]; \
	} \
	static ENUM_TYPE fromString(const std::string& str) { \
		static const std::string STRARR[] = {ENUM_STRFILL(VALUES)}; \
		static const StringMatcher<ENUM_TYPE> matcher(STRARR, -1); \
		return matcher.match(str); \
	} \
	static bool isValid(Enum val) {return (int)val < count() && val >= 0;} \
	Enum val; \
	ENUM_TYPE(): val((Enum)0) { } \
	ENUM_TYPE(int val): val((Enum)val) { } \
	ENUM_TYPE& operator=(Enum rhs) {val = rhs; return *this;} \
	ENUM_TYPE& operator++() {val = (Enum)(val + 1); return *this;} \
	operator Enum() const {return val;} \
	const std::string& toString() const {return toString(*this);} \
	bool isValid() const {return isValid(val);}


#define ENUM_STRFILL(VALUES) BOOST_PP_LIST_FOR_EACH_I( ENUM_STRCOM, _, VALUES )
#define ENUM_STRCOM(R, DATA, I, ELEM) ENUM_STR( ELEM ),
#define ENUM_STR(TO_STR) #TO_STR

// some standard useful enums
enum class Result {SUCCESS, FAILURE};
enum class Happen {NO, YES};


#endif // ENUM_H
