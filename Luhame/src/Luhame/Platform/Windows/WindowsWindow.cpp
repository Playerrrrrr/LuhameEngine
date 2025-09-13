#include"pch.h"
#include"glad/glad.h"
#include "WindowsWindow.h"
#include"Luhame/Core/Event/ApplicationEvent.h"
#include"Luhame/Core/Event/KeyEvent.h"
#include"Luhame/Core/Event/MouseEvent.h"
#include"imgui.h"
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
		glfwPollEvents();
		glfwSwapBuffers(m_window);
		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		glfwSetCursor(m_window, m_imgui_mouse_cursors[imgui_cursor] ? m_imgui_mouse_cursors[imgui_cursor] : m_imgui_mouse_cursors[ImGuiMouseCursor_Arrow]);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//使用高版本的opengl
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(
			static_cast<int>(props.m_width),
			static_cast<int>(props.m_height),
			props.m_title.c_str(),
			nullptr,
			nullptr
		);

		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		LH_CORE_ASSERT(status, "Failed to initialize Glad!");
		glfwSetWindowUserPointer(m_window, static_cast<void*>(&m_window_data));
		set_VSync(true);


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


		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint)
		{
			window_data* data = static_cast<window_data*>(glfwGetWindowUserPointer(window));

			key_typed_event event((int)codepoint);
			data->m_event_callback_fn(event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			auto& data = *((window_data*)glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				key_pressed_event event(key, 0);
				data.m_event_callback_fn(event);
				break;
			}
			case GLFW_RELEASE:
			{
				key_released_event event(key);
				data.m_event_callback_fn(event);
				break;
			}
			case GLFW_REPEAT:
			{
				key_pressed_event event(key, 1);
				data.m_event_callback_fn(event);
				break;
			}
			}
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			auto& data = *((window_data*)glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				mouse_button_pressed_event event(button);
				data.m_event_callback_fn(event);
				break;
			}
			case GLFW_RELEASE:
			{
				mouse_button_released_event event(button);
				data.m_event_callback_fn(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			auto& data = *((window_data*)glfwGetWindowUserPointer(window));

			mouse_scrolled_event event((float)xOffset, (float)yOffset);
			data.m_event_callback_fn(event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
		{
			auto& data = *((window_data*)glfwGetWindowUserPointer(window));

			mouse_moved_event event((float)x, (float)y);
			data.m_event_callback_fn(event);
		});
		//光标
		m_imgui_mouse_cursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_imgui_mouse_cursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		m_imgui_mouse_cursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
		m_imgui_mouse_cursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
		m_imgui_mouse_cursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		m_imgui_mouse_cursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		m_imgui_mouse_cursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		m_imgui_mouse_cursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

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