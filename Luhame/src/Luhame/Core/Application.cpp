#include"pch.h"
#include "Application.h"
#include"Luhame/Core/Log.h"

namespace Luhame {


	Application::Application()
	{
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

	void Application::Run()
	{
		while (true) {
			m_window->on_update();
		}
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
		LH_CORE_TRACE("{}", event);
	}

	bool Application::on_window_resize(window_resize_event& event)
	{
		return false;
	}

	bool Application::on_window_close(window_closed_event& event)
	{
		return false;
	}

}