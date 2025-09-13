#include"pch.h"
#include "Application.h"
#include"Luhame/Core/Event/KeyEvent.h"
#include"Luhame/Core/Event/MouseEvent.h"
#include"Luhame/Core/Log.h"
#include"Luhame/Renderer/Renderer.h"
#include"ImGui/backends/imgui_impl_opengl3.h"

#include"GLFW/glfw3.h"
#include"imgui/backends/imgui_impl_glfw.h"

#include"Luhame/Renderer/Framebuffer.h"
#include"imgui/imgui.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>

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

		::Luhame::renderer::init();//依赖window::create时导入opengl函数
	}

	Application::~Application()
	{

	}
	void Application::on_init() {
		IMGUI_CHECKVERSION();
		LH_CORE_ASSERT(ImGui::CreateContext(),"nullptr");
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		ImFont* pfont = io.Fonts->AddFontFromFileTTF("../Luhame/assets/fonts/Segoe UI.ttf");
		//io.FontDefault = pfont;
		// io.Fonts->Fonts.back()
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application* app = Application::get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app->get_window().get_native_window());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
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

		time_step m_time;
		while (m_runing) {
			on_begin_frame();
			

			for (layer* t_layer : m_layer_stack) {
				t_layer->on_update(m_time);//提交渲染数据
			}

			for (auto& t : m_layer_stack) {
				t->on_ui_render();
			}
			LH_CORE_INFO("Begin Render");
			renderer::get().wait_and_render();//渲染
			LH_CORE_INFO("End Render");

			on_end_frame();

			m_window->on_update();
			m_time.update();
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

	void Application::on_begin_frame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		{//将主窗口作为docking space
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			// 获取视口并设置窗口位置和大小
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			// 推送样式变量
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			// 创建主docking窗口
			bool open = true;
			ImGui::Begin("DockingSpace", &open, window_flags);
			ImGui::PopStyleVar(3);

			// 创建docking空间
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

			ImGui::End();
		}
	}

	void Application::on_end_frame()
	{
		ImGuiIO& io = ImGui::GetIO();

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
	}

	std::string Application::open_file(const std::string& filter) const
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_window->get_native_window());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "All\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
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