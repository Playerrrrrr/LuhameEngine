#include"pch.h"
#include "Application.h"

namespace Luhame {

	Application::Application()
	{
		m_window = std::unique_ptr<window>(window::create(
			window_props{"Luhame Engine"}
		));
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (true) {
			m_window->on_update();
		}
	}

}