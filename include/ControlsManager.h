#ifndef CONTROLS_MANAGER_H
#define CONTROLS_MANAGER_H

#include "util/Enum.h"
class YamlWrapper;
namespace irr { struct SEvent; }
#include <map>
#include <vector>

class ControlsManager {
public:
	ControlsManager();

	void parseControls(YamlWrapper& yaml);

	void readInput(const irr::SEvent& input);

	bool isDown(const std::string& action);
	bool isDown(size_t action);

private:
	int parseControl(const std::string& name);

	struct Action {
		std::string name;
		bool isDown;
		std::vector<int> keys;
	};
	std::vector<Action> actions;
	std::unordered_map<std::string, size_t> strToActionMap;
	std::unordered_map<int, size_t> keyToActionMap;
};

#endif // CONTROLS_MANAGER_H
