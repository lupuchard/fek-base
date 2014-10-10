#include "catch.hpp"
#include "ResourceManager.h"

TEST_CASE("Basic test of the ResourceManager", "[resources]") {
	ResourceManager resourceManager("data/extensions.yaml");
	resourceManager.scanResources("tests/World/");

	const TexRes* res1 = resourceManager.peekResource<ResType::TEX>("Test");
	REQUIRE(res1 != nullptr);
	REQUIRE(res1->isLoaded() == false);
	REQUIRE(res1->getFilename() == "tests/World/textures/test.png");
	REQUIRE(res1->getNameID() == "test");

	const TexRes* res2 = resourceManager.peekResource<ResType::TEX>("Doesn't Exist");
	REQUIRE(res2 == nullptr);

	YamlRes* res3 = resourceManager.getResource<ResType::YAML>("~~ITeMS~~");
	REQUIRE(res3 != nullptr);
	const YamlWrapper& yaml = res3->getYaml();
	REQUIRE(yaml.read<String>("Base") == "fork");
}
