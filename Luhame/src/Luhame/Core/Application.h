#pragma once
#include "Luhame/Core/Core.h"
#include"Luhame/Core/window.h"
#include"Luhame/Core/Event/ApplicationEvent.h"

namespace Luhame {

	class LUHAME_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		virtual void OnInit() {}
		virtual void OnShutdown() {}
		virtual void OnUpdate() {}

		virtual void on_event(event& event);
	private:

		bool on_window_resize(window_resize_event& event);
		bool on_window_close(window_closed_event& event);

		std::unique_ptr<window> m_window;
	};

	// Implemented by CLIENT
	Application* CreateApplication();
}