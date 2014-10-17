#include "ControlsManager.h"

#include "util/YamlWrapper.h"
#include <easylogging++.h>
#include <irrlicht/IEventReceiver.h>

ControlsManager::ControlsManager() {
	actions.emplace_back(); // zero is null
}

void ControlsManager::parseControls(YamlWrapper& yaml) {
	YAML::Node node = yaml.getNode();
	for (auto iter = node.begin(); iter != node.end(); ++iter) {
		YamlWrapper actionYaml(*iter);
		String name = actionYaml.read<String>("Name", "", "Control action requires name.");
		name = Str::simplify(name);
		actions.emplace_back();
		Action& action = actions.back();
		action.name = name;
		action.isDown = false;
		strToActionMap[name] = actions.size() - 1;

		String controlName = actionYaml.read<String>("Default", "");
		if (controlName != "") {
			int key = parseControl(controlName);
			if (key != -1) {
				action.keys.push_back(key);
				keyToActionMap[key] = actions.size() - 1;
			}
		}
	}
}

using namespace irr;
const std::unordered_map<String, EKEY_CODE> KEY_CODE_MAP = {
	{"lbutton", KEY_LBUTTON}, {"rbutton", KEY_RBUTTON}, {"cancel", KEY_CANCEL},
	{"mbutton", KEY_MBUTTON}, {"xbutton1", KEY_XBUTTON1}, {"xbutton2", KEY_XBUTTON2},
	{"back", KEY_BACK}, {"tab", KEY_TAB}, {"clear", KEY_CLEAR}, {"return", KEY_RETURN},
	{"kana", KEY_KANA}, {"hangul", KEY_HANGUL}, {"junja", KEY_JUNJA},
	{"final", KEY_FINAL}, {"hanja", KEY_HANJA}, {"kanji", KEY_KANJI}, {"escape", KEY_ESCAPE},
	{"convert", KEY_CONVERT}, {"nonconvert", KEY_NONCONVERT}, {"accept", KEY_ACCEPT},
	{"modechange", KEY_MODECHANGE}, {"space", KEY_SPACE}, {"prior", KEY_PRIOR},
	{"next", KEY_NEXT}, {"end", KEY_END}, {"home", KEY_HOME},
	{"left", KEY_LEFT}, {"up", KEY_UP}, {"right", KEY_RIGHT}, {"down", KEY_DOWN},
	{"select", KEY_SELECT}, {"print", KEY_PRINT}, {"execut", KEY_EXECUT},
	{"snapshot", KEY_SNAPSHOT}, {"insert", KEY_INSERT}, {"delete", KEY_DELETE},
	{"capital", KEY_CAPITAL}, {"numlock", KEY_NUMLOCK}, {"scroll", KEY_SCROLL},
	{"help", KEY_HELP}, {"pause", KEY_PAUSE}, 
	{"0", KEY_KEY_0}, {"key0", KEY_KEY_0}, {"1", KEY_KEY_1}, {"key1", KEY_KEY_1},
	{"2", KEY_KEY_2}, {"key2", KEY_KEY_2}, {"3", KEY_KEY_3}, {"key3", KEY_KEY_3},
	{"4", KEY_KEY_4}, {"key4", KEY_KEY_4}, {"5", KEY_KEY_5}, {"key5", KEY_KEY_5},
	{"6", KEY_KEY_6}, {"key6", KEY_KEY_6}, {"7", KEY_KEY_7}, {"key7", KEY_KEY_7},
	{"8", KEY_KEY_8}, {"key8", KEY_KEY_8}, {"9", KEY_KEY_9}, {"key9", KEY_KEY_9},
	{"a", KEY_KEY_A}, {"keya", KEY_KEY_A}, {"b", KEY_KEY_B}, {"keyb", KEY_KEY_B},
	{"c", KEY_KEY_C}, {"keyc", KEY_KEY_C}, {"d", KEY_KEY_D}, {"keyd", KEY_KEY_D},
	{"e", KEY_KEY_E}, {"keye", KEY_KEY_E}, {"f", KEY_KEY_F}, {"keyf", KEY_KEY_F},
	{"g", KEY_KEY_G}, {"keyg", KEY_KEY_G}, {"h", KEY_KEY_H}, {"keyh", KEY_KEY_H},
	{"i", KEY_KEY_I}, {"keyi", KEY_KEY_I}, {"j", KEY_KEY_J}, {"keyj", KEY_KEY_J},
	{"k", KEY_KEY_K}, {"keyk", KEY_KEY_K}, {"l", KEY_KEY_L}, {"keyl", KEY_KEY_L},
	{"m", KEY_KEY_M}, {"keym", KEY_KEY_M}, {"n", KEY_KEY_N}, {"keyn", KEY_KEY_N},
	{"o", KEY_KEY_O}, {"keyo", KEY_KEY_O}, {"p", KEY_KEY_P}, {"keyp", KEY_KEY_P},
	{"q", KEY_KEY_Q}, {"keyq", KEY_KEY_Q}, {"r", KEY_KEY_R}, {"keyr", KEY_KEY_R},
	{"s", KEY_KEY_S}, {"keys", KEY_KEY_S}, {"t", KEY_KEY_T}, {"keyt", KEY_KEY_T},
	{"u", KEY_KEY_U}, {"keyu", KEY_KEY_U}, {"v", KEY_KEY_V}, {"keyv", KEY_KEY_V},
	{"w", KEY_KEY_W}, {"keyw", KEY_KEY_W}, {"x", KEY_KEY_X}, {"keyx", KEY_KEY_X},
	{"y", KEY_KEY_Y}, {"keyy", KEY_KEY_Y}, {"z", KEY_KEY_Z}, {"keyz", KEY_KEY_Z},
	{"lwin", KEY_LWIN}, {"rwin", KEY_RWIN}, {"apps", KEY_APPS}, {"sleep", KEY_SLEEP},
	{"numpad0", KEY_NUMPAD0}, {"numpad1", KEY_NUMPAD1}, {"numpad2", KEY_NUMPAD2},
	{"numpad3", KEY_NUMPAD3}, {"numpad4", KEY_NUMPAD4}, {"numpad5", KEY_NUMPAD5},
	{"numpad6", KEY_NUMPAD6}, {"numpad7", KEY_NUMPAD7}, {"numpad8", KEY_NUMPAD8},
	{"numpad9", KEY_NUMPAD9},
	{"multiply", KEY_MULTIPLY}, {"add", KEY_ADD}, {"separator", KEY_SEPARATOR},
	{"f1", KEY_F1}, {"f2", KEY_F2}, {"f3", KEY_F3}, {"f4", KEY_F4}, {"f5", KEY_F5},
	{"f6", KEY_F6}, {"f7", KEY_F7}, {"f8", KEY_F8}, {"f9", KEY_F9}, {"f10", KEY_F10},
	{"f11", KEY_F11}, {"f12", KEY_F12}, {"f13", KEY_F13}, {"f14", KEY_F14}, {"f15", KEY_F15},
	{"f16", KEY_F16}, {"f17", KEY_F17}, {"f18", KEY_F18}, {"f19", KEY_F19}, {"f20", KEY_F20},
	{"f21", KEY_F21}, {"f22", KEY_F22}, {"f23", KEY_F23}, {"f24", KEY_F24},
	{"menu", KEY_MENU}, {"lmenu", KEY_LMENU}, {"rmenu", KEY_RMENU},
	{"shift", KEY_SHIFT}, {"lshift", KEY_LSHIFT}, {"rshift", KEY_RSHIFT},
	{"control", KEY_CONTROL}, {"lcontrol", KEY_LCONTROL}, {"rcontrol", KEY_RCONTROL},
	{"plus", KEY_PLUS}, {"minus", KEY_MINUS}, {"comma", KEY_COMMA}, {"period", KEY_PERIOD},
	{"oem1", KEY_OEM_1}, {"oem2", KEY_OEM_2}, {"oem3", KEY_OEM_3}, {"oem4", KEY_OEM_4},
	{"oem5", KEY_OEM_5}, {"oem6", KEY_OEM_6}, {"oem7", KEY_OEM_7}, {"oem8", KEY_OEM_8},
	{"oemax", KEY_OEM_AX}, {"oem102", KEY_OEM_102}, {"oemclear", KEY_OEM_CLEAR},
	{"attn", KEY_ATTN}, {"crsel", KEY_CRSEL}, {"exsel", KEY_EXSEL}, {"ereof", KEY_EREOF},
	{"play", KEY_PLAY}, {"zoom", KEY_ZOOM}, {"pa1", KEY_PA1},
};
int ControlsManager::parseControl(StringRef name) {
	String controlName = Str::simplify(name);
	auto iter = KEY_CODE_MAP.find(controlName);
	if (iter == KEY_CODE_MAP.end()) {
		return -1;
	}
	return iter->second;
}

void ControlsManager::readInput(const irr::SEvent& input) {
	if (input.EventType != irr::EET_KEY_INPUT_EVENT) return;
	auto iter = keyToActionMap.find(input.KeyInput.Key);
	if (iter == keyToActionMap.end()) return;
	size_t action = iter->second;
	DCHECK(action < actions.size());
	actions[action].isDown = input.KeyInput.PressedDown;
}

bool ControlsManager::isDown(StringRef controlName) {
	auto iter = strToActionMap.find(controlName);
	if (iter == strToActionMap.end()) return false;
	size_t action = iter->second;
	DCHECK(action < actions.size());
	return actions[action].isDown;
}
bool ControlsManager::isDown(size_t action) {
	if (action >= actions.size()) return false;
	return actions[action].isDown;
}
