#include"pch.h"
#include "Application.h"
#include"Luhame/Core/Event/KeyEvent.h"
#include"Luhame/Core/Event/MouseEvent.h"
#include"Luhame/Core/Log.h"
#include"GLFW/glfw3.h"
#include"Luhame/Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include"imgui/backends/imgui_impl_glfw.h"

namespace Luhame {


	Application::Application()
	{
		LH_CORE_ASSERT(!is_init, "application must be single")
		is_init = true;
		s_instance = this;
		m_window = std::unique_ptr<window>(window::create(
			window_props{"Luhame Engine"}
		));
		m_window->set_event_callback(
			std::bind(&Application::on_event, this, std::placeholders::_1)
		);
	}

	Application::~Application()
	{

	}
	void Application::on_init() {
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		//处理后端数据，比如键盘和鼠标的输入
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			//离屏？
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(m_window->get_native_window()), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void Application::on_shut_down()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void Application::run()
	{

		on_init();

		ImGuiIO& io = ImGui::GetIO();


		while (m_runing) {

			float time = (float)glfwGetTime();
			io.DeltaTime = m_time > 0.0 ? (time - m_time) : (1.0f / 60.0f);
			m_time = time;

			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			for (layer* t_layer : m_layer_stack) {
				t_layer->on_update();
			}
			Application& app = *Application::get();
			io.DisplaySize = ImVec2(app.get_window().get_width(), app.get_window().get_height());

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
			m_window->on_update();
		}
		on_shut_down();
	}

	void Application::on_event(event& event)
	{
		event_dispatcher dispatcher(event);
		dispatcher.dispatch<window_resize_event>(
			std::bind(&Application::on_window_resize, this, std::placeholders::_1)
		);
		dispatcher.dispatch<window_closed_event>(
			std::bind(&Application::on_window_close, this, std::placeholders::_1)
		);

		//从下到上传递事件，事件被处理后就不传了
		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();) {
			(*--it)->on_event(event);
			if (event.get_handled_state()) {
				break;
			}
		}
	}

	void Application::push_layer(layer* t_layer)
	{
		m_layer_stack.push_layer(t_layer);
		t_layer->on_attach();
	}

	void Application::push_over_layer(layer* t_layer)
	{
		m_layer_stack.push_over_layer(t_layer);
		t_layer->on_attach();
	}

	bool Application::on_window_resize(window_resize_event& event)
	{
		return false;
	}

	bool Application::on_window_close(window_closed_event& event)
	{
		m_runing = false;
		return false;
	}

}