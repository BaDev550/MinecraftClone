#pragma once

#include <core/Window.h>
#include <string>

class Renderer
{
private:
	unsigned int w_Width{ 800 };
	unsigned int w_Height{ 600 };
	std::string w_Title = "OpenGL Window";
	Window window;

	int wfb_Width{};
	int wfb_Height{};
public:
	Renderer() {}
	~Renderer();

	void start(unsigned int width, unsigned int height, const char* title);
	void update();
	void render();
	void processInput();
};

