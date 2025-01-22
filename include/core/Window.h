#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

class Window
{
private:
	GLFWwindow* window;
	int fb_width, fb_height;

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
public:
	Window() {};
	~Window();

	void setProperty(int property, int value);
	bool CreateWindow(int width, int height, const char* title);
	void ClearScreen();
	void SwapBuffers();
	bool ShouldClose();

	GLFWwindow* getWindow() { return window; }
	int getFBWidth() { return fb_width; }
	int getFBHeight() { return fb_height; }
};

