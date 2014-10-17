#include <catch.hpp>
#include "ControlsManager.h"

#include "util/YamlWrapper.h"
#include <irrlicht/IEventReceiver.h>

TEST_CASE("Basic test of the ControlsManager", "[controls]") {
	ControlsManager controls;
	YamlWrapper yaml(YAML::LoadFile("packages/TestPackage/Controls.yaml"));
	controls.parseControls(yaml);

	REQUIRE(controls.isDown("poops") == false);

	irr::SEvent event;
	event.EventType = irr::EET_KEY_INPUT_EVENT;
	event.KeyInput.Char = 0;
	event.KeyInput.Key  = irr::KEY_UP;
	event.KeyInput.Control = false;
	event.KeyInput.Shift   = false;
	event.KeyInput.PressedDown = true;
	controls.readInput(event);
	REQUIRE(controls.isDown("forward") == true);
}