#pragma once
#include "Menu.h"

class OptionMenu : public Menu
{
private:
	Sound m_music;
	Sound m_sound;

	const enum class BType
	{
		Up,
		Left,
		Right,
		Crouch,
		Grab,
		Reflector,
		Size
	};

	Object[]*
public:
	OptionMenu(MenuStack &menuStack);
};