#include <catch.hpp>
#include "ResourceManager.h"

TEST_CASE("Basic test of the ResourceManager", "[resources]") {
	ResourceManager resourceManager("data/extensions.yaml");
	resourceManager.scanPackages("packages");

	const TexRes* res1 = resourceManager.peekResource<ResType::TEX>("testpackage:Test");
	REQUIRE(res1 != nullptr);
	REQUIRE(res1->isLoaded() == false);
	REQUIRE(res1->getFilename() == "packages/TestPackage/textures/test.png");
	REQUIRE(res1->getNameID() == "testpackage:test");

	const TexRes* res2 = resourceManager.peekResource<ResType::TEX>("Doesn't Exist");
	REQUIRE(res2 == nullptr);

	YamlRes* res3 = resourceManager.getResource<ResType::YAML>("~~tEsTpAcKaGe~~:~~iTeMs~~");
	REQUIRE(res3 != nullptr);
	const YamlWrapper& yaml = res3->getYaml();
	REQUIRE(yaml.read<String>("Base") == "fork");

	YamlRes* res4 = resourceManager.getResource<ResType::YAML>("testpackageworld:blah");
	REQUIRE(res4 != nullptr);
}
