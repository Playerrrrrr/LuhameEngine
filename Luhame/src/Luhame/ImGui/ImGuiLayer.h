#pragma once
#include"Luhame/Core/Layer.h"

#include"Luhame/Core/Event/KeyEvent.h"
#include"Luhame/Core/Event/MouseEvent.h"
#include"Luhame/Core/Event/ApplicationEvent.h"

namespace Luhame {
	class LUHAME_API imgui_layer:public layer
	{
	public:
		imgui_layer();
		imgui_layer(const std::string& name);
		virtual ~imgui_layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void on_update(const time_step&) override;
		virtual void on_event(event& event)override;
		virtual void on_ui_render() override{};
		void begin() {}
		void end(){}
	private:
		bool on_mouse_button_pressed_event(mouse_button_pressed_event& e);
		bool on_mouse_button_released_event(mouse_button_released_event& e);
		bool on_mouse_moved_event(mouse_moved_event& e);
		bool on_mouse_scrolled_event(mouse_scrolled_event& e);
		bool on_key_pressed_event(key_pressed_event& e);
		bool on_key_released_event(key_released_event& e);
		bool on_window_resize_event(window_resize_event& e);
		bool on_key_typed_event(key_typed_event& e);

	private:
		float m_time = 0.0f;
	};

  
}

