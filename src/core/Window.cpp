#include <core/Window.h>
#include <iostream>

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::setProperty(int property, int value)
{
	switch (property)
	{
	case 0:
		glfwSwapInterval(value);
		break;
	case 1:
		glfwWindowHint(GLFW_RESIZABLE, value);
		break;
	default:
		std::cout << "No property found with the value: " << property << std::endl;
		break;
	}
}

bool Window::CreateWindow(int width, int height, const char* title)
{
	if (!glfwInit()) {
		std::cout << "ERROR::GLFW::Init failed at: " __FILE__ " line: " << __LINE__ << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, true);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		std::cout << "ERROR::GLFW::Window creation failed at: " __FILE__ " line: " << __LINE__ << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwGetFramebufferSize(window, &fb_width, &fb_height);
	glfwSetErrorCallback([](int error, const char* description) {
		std::cout << "GLFW Error: " << description << std::endl;
	});

	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		return true;
	}
	else
	{
		std::cout << "ERROR::GLAD::Init failed at: " __FILE__ " line: " << __LINE__ << std::endl;
		glfwTerminate();
		return false;
	}
	glViewport(0, 0, width, height);

	return true;
}

void Window::ClearScreen()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwGetFramebufferSize(window, &fb_width, &fb_height);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::processInput()
{
	if (input.isKeyPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}