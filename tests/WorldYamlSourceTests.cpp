#include "catch.hpp"
#include "WorldYamlSource.h"

TEST_CASE("WorldSource", "Yaml") {
	WorldYamlSource source("tests/World/");

	Item* item1 = source.getNewItem();
	Item* item2 = source.getNewItem();
	Item* item3 = source.getNewItem();
	REQUIRE(source.getNewItem() == nullptr);

	REQUIRE(item1 != nullptr);
	REQUIRE(boost::get<String>(item1->getValue(1)) == "fork");
	REQUIRE(item1->getValueT<String>("name")   == "fork");
	REQUIRE(item1->getValueT<int>("weight") == 10);

	REQUIRE(item2 != nullptr);
	REQUIRE(item2->getValueT<String>("name")   == "carrot");
	REQUIRE(item2->getPosition() == Coord(10, 20));

	REQUIRE(item3 != nullptr);
	REQUIRE(item3->getValueT<int>("nutrition") == 5);
	REQUIRE(item3->getValueT<int>("remaining") == 50);
}