#include "pch.h"
#include "WindowsInput.h"
#include "WindowsWindow.h"

#include "Luhame/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Luhame {
	input* input::s_instance = new windows_input;

	bool windows_input::is_key_pressed_impl(int keycode)
	{
		auto& window = static_cast<windows_window&>(Application::get()->get_window());
		auto state = glfwGetKey(static_cast<GLFWwindow*>(window.get_native_window()), keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool windows_input::is_mouse_button_pressed_impl(int button)
	{
		auto& window = static_cast<windows_window&>(Application::get()->get_window());

		auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(window.get_native_window()), button);
		return state == GLFW_PRESS;
	}

	float windows_input::get_mouse_x_impl()
	{
		auto& window = static_cast<windows_window&>(Application::get()->get_window());

		double xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.get_native_window()), &xpos, &ypos);

		return (float)xpos;
	}

	float windows_input::get_mouse_y_impl()
	{
		auto& window = static_cast<windows_window&>(Application::get()->get_window());

		double xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.get_native_window()), &xpos, &ypos);

		return (float)ypos;
	}

}