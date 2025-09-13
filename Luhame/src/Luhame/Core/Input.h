#pragma once
#include"Luhame/Core/KeyKode.h"
namespace Luhame {


	class input
	{
	public:
		static bool is_key_pressed(int keycode) { return s_instance->is_key_pressed_impl(keycode); }

		inline static bool is_mouse_button_pressed(int button) { return s_instance->is_mouse_button_pressed_impl(button); }
		inline static float get_mouse_x() { return s_instance->get_mouse_x_impl(); }
		inline static float get_mouse_y() { return s_instance->get_mouse_y_impl(); }
		static std::unordered_map<int, int> key_map;
	protected:
		virtual bool is_key_pressed_impl(int keycode) = 0;
		virtual bool is_mouse_button_pressed_impl(int button) = 0;
		virtual float get_mouse_x_impl() = 0;
		virtual float get_mouse_y_impl() = 0;
	private:
		static input* s_instance;
	};

}