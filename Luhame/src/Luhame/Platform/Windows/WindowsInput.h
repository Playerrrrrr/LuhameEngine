#pragma once

#include "Luhame/Core/Input.h"

namespace Luhame {

	class windows_input : public input
	{
	protected:
		//glfw code
		virtual bool is_key_pressed_impl(int keycode) override;

		virtual bool is_mouse_button_pressed_impl(int button) override;
		virtual float get_mouse_x_impl() override;
		virtual float get_mouse_y_impl() override;
	};

}