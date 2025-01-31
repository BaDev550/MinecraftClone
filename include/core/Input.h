#pragma once
#include <glfw/glfw3.h>
#include <unordered_map>

class Input
{
private:

public:
	Input();

	bool isKeyPressed(int keycode, bool shift = false, bool control = false);
	bool isKeyReleased(int keycode);
	bool isMouseButtonPressed(int button);
	bool isMouseButtonClicked(int button);
	bool isMouseButtonReleased(int button);

	void update();
};

