#include <core/Input.h>

Input::Input()
{

}

bool Input::isKeyPressed(int keycode, bool shift, bool control)
{
	if (shift)
	{
		return (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(glfwGetCurrentContext(), keycode) == GLFW_PRESS);
	}
    if (control) {
		return (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(glfwGetCurrentContext(), keycode) == GLFW_PRESS);
    }
    return (glfwGetKey(glfwGetCurrentContext(), keycode) == GLFW_PRESS);
}

bool Input::isKeyReleased(int keycode)
{
    return (glfwGetKey(glfwGetCurrentContext(), keycode) == GLFW_RELEASE);
}

bool Input::isMouseButtonPressed(int button)
{
    return (glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS);
}

bool Input::isMouseButtonClicked(int button)
{
    return false;
}

bool Input::isMouseButtonReleased(int button)
{
    return (glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_RELEASE);
}

void Input::update()
{
}
