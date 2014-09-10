#include "catch.hpp"
#include "PropertySystem.h"

TEST_CASE("Property", "System") {
	PropertySystem sys;

	sys.add("butts"   , Type::INT);
	sys.add("is butt" , Type::BOOL);
	sys.add("fartname", Type::STRING);

	REQUIRE(sys.size() == 4);
	REQUIRE(sys[1].name == "butts");
	REQUIRE(sys["is butt"].type == Type::BOOL);
	REQUIRE(sys.getIndex("fartname") == 3);
}