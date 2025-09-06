#pragma once


#include "Luhame/Core/Core.h"
#include"Luhame/Core/window.h"
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
	private:
		std::unique_ptr<window> m_window;
	};

	// Implemented by CLIENT
	Application* CreateApplication();
}