#include <core/Input.h>

bool Input::isKeyPressed(int keycode)
{
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

bool Input::isMouseButtonReleased(int button)
{
    return (glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_RELEASE);
}

float Input::getMouseX()
{
    return 0.0f;
}

float Input::getMouseY()
{
    return 0.0f;
}
