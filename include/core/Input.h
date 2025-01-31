#pragma once
#include <glfw/glfw3.h>

class Input
{
public:
	static bool isKeyPressed(int keycode, bool shift = false, bool control = false);
	static bool isKeyReleased(int keycode);
	static bool isMouseButtonPressed(int button);
	static bool isMouseButtonReleased(int button);

	static float getMouseX();
	static float getMouseY();
};

