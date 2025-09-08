#pragma once
#include "Luhame/Core/Core.h"
#include"Luhame/Core/window.h"
#include"Luhame/Core/Event/ApplicationEvent.h"
#include"Luhame/Core/LayerStack.h"

namespace Luhame {

	class LUHAME_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		virtual void on_init();
		virtual void on_shut_down();
		virtual void on_update() {}

		virtual void on_event(event& event);

		void push_layer(layer* t_layer);
		void push_over_layer(layer* t_layer);

		window& get_window() { return *m_window.get(); }
		static Application* get() { return s_instance; }
	private:

		bool on_window_resize(window_resize_event& event);
		bool on_window_close(window_closed_event& event);

		std::unique_ptr<window> m_window;
		layer_stack m_layer_stack;
		bool m_runing = true;
		inline static bool is_init = false;
		inline static Application* s_instance;

		float m_time;
	};

	// Implemented by CLIENT
	Application* CreateApplication();
}