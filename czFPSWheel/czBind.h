#pragma once
#include "czApi.h"
enum modifierKeys
{
	alt = 1,
	shift = 2,
	control = 4
};
enum class bindState
{

	up = 0,
	down = 1,
	held = 2,
	pressed = 3
};

class czBind
{
private:
	Timer tDown;
public:

	bindState state;
	bool asyncKeyDown();
	void update(WPARAM Keycode, UINT WindowsMessage);
	czBind(std::string name, std::string desc, int default_key = 0, int default_modifier = 0, int default_WM = WM_KEYDOWN);
	czVar* bindVar;
};