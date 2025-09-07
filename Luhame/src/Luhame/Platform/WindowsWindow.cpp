#include"pch.h"
#include "WindowsWindow.h"
#include"Luhame/Core/Log.h"
#include"Luhame/Core/Event/ApplicationEvent.h"

namespace Luhame {

	static void glfw_error_callback(int error, const char* description) {
		LH_CORE_ERROR("GLFW Error({0}): {1}", error, description);
	}

	window* window::create(const window_props& props) {
		return  new windows_window(props);
	}

	windows_window::windows_window(const window_props& props)
	{
		init(props);
	}
	windows_window::~windows_window()
	{
	}
	void windows_window::on_update()
	{
		LH_CORE_INFO("window update");
		glfwPollEvents();
	}
	void windows_window::init(const window_props& props)
	{
		static bool is_glfw_init = false;
		m_window_data.m_height = props.m_height;
		m_window_data.m_width = props.m_width;
		m_window_data.m_title = props.m_title;
		LH_CORE_INFO("Creating window {0} ({1}, {2})", props.m_title, props.m_width, props.m_height);

		if (!is_glfw_init) {
			int success = glfwInit();
			LH_CORE_ASSERT(success, "failed to initialize glfw");
			glfwSetErrorCallback(glfw_error_callback);
			is_glfw_init = true;
		}   
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_window = glfwCreateWindow(
			static_cast<int>(props.m_width),
			static_cast<int>(props.m_height),
			props.m_title.c_str(),
			nullptr,
			nullptr
		);
		set_VSync(true);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, static_cast<void*>(&m_window_data));
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			window_data* data = static_cast<window_data*>(glfwGetWindowUserPointer(window));
			data->m_width = width;
			data->m_height = height;
			window_resize_event event(width, height);
			data->m_event_callback_fn(event);
		});
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			window_data* data = static_cast<window_data*>(glfwGetWindowUserPointer(window));
			window_closed_event event;
			data->m_event_callback_fn(event);
		});
	}


	void windows_window::shut_down()
	{
	}
	void windows_window::set_VSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_window_data.VSync = enabled;
	}
}