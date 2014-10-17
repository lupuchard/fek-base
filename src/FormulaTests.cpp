#include <catch.hpp>
#include "Formula.h"

TEST_CASE("Basic math of the Formula", "[properties][formulas]") {
	Formula formula;

	Result res = formula.parse("2 + 3 * 4");
	REQUIRE(res == Result::SUCCESS);
	REQUIRE(boost::get<int>(formula.evaluate()) == 14);

	res = formula.parse("( 23.5- (22+1))/0.25");
	REQUIRE(res == Result::SUCCESS);
	REQUIRE(boost::get<float>(formula.evaluate()) == 2.f);

	res = formula.parse("4^3^2");
	REQUIRE(res == Result::SUCCESS);
	REQUIRE(boost::get<int>(formula.evaluate()) == 262144);

	res = formula.parse("19 % 3");
	REQUIRE(res == Result::SUCCESS);
	REQUIRE(boost::get<int>(formula.evaluate()) == (19 % 3));
}

TEST_CASE("Basic failure of the Formula", "[properties][formulas]") {
	Formula formula;
	Result res;
	res = formula.parse("3 % 2.5");
	REQUIRE(res == Result::FAILURE);
	res = formula.parse("2d4 + 6");
	REQUIRE(res == Result::FAILURE);
	res = formula.parse("(6 + 2))");
	REQUIRE(res == Result::FAILURE);
	res = formula.parse("((6 + 2)");
	REQUIRE(res == Result::FAILURE);
	res = formula.parse("6 + ");
	REQUIRE(res == Result::FAILURE);
	res = formula.parse("* * *");
	REQUIRE(res == Result::FAILURE);
	res = formula.parse("3 ~= 2");
	REQUIRE(res == Result::FAILURE);
}

TEST_CASE("Basic string test of the Formula", "[properties][formulas]") {
	Formula formula;

	Result res = formula.parse("\"eat \" + 6 + \" hotdogs\"");
	REQUIRE(res == Result::SUCCESS);
	REQUIRE(boost::get<String>(formula.evaluate()) == "eat 6 hotdogs");

	res = formula.parse("\"eat\" * 6");
	REQUIRE(res == Result::FAILURE);

	res = formula.parse("\"\"\"");
	REQUIRE(res == Result::FAILURE);
}

TEST_CASE("Basic boolean operation tests of the Formula", "[properties][formulas]") {
	Formula formula;
	
	Result res = formula.parse("3 == 1 + 2");
	REQUIRE(res == Result::SUCCESS);
	REQUIRE(boost::get<int>(formula.evaluate()) == true);

	res = formula.parse("\"hotdog\" != \"hotdog\"");
	REQUIRE(res == Result::SUCCESS);
	REQUIRE(boost::get<int>(formula.evaluate()) == false);

	res = formula.parse("4 * 3 >= 12 && 4 > 3");
	REQUIRE(res == Result::SUCCESS);
	REQUIRE(boost::get<int>(formula.evaluate()) == true);

	res = formula.parse("2 < 2 || 2 <= 2");
	REQUIRE(res == Result::SUCCESS);
	REQUIRE(boost::get<int>(formula.evaluate()) == true);

	res = formula.parse("2==3||4>4");
	REQUIRE(res == Result::SUCCESS);
	REQUIRE(boost::get<int>(formula.evaluate()) == false);

	res = formula.parse("(2 == 1 * 2) * 3.1 + 4.2");
	REQUIRE(res == Result::SUCCESS);
	float val = boost::get<float>(formula.evaluate());
	REQUIRE(std::abs(val - 7.3) < 0.0001);
}