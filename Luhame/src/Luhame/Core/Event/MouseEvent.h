#pragma once
#include"Event.h"

namespace Luhame {

	class LUHAME_API mouse_moved_event : public event {
	public:
		mouse_moved_event(float x,float y,float dx,float dy)
		:m_mouse_x(x),m_mouse_y(y),m_mouse_dx(dx), m_mouse_dy(dy){}

		inline float get_x() const { return m_mouse_x; }
		inline float get_y() const { return m_mouse_y; }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse| EventCategoryInput)

	private:
		float m_mouse_x, m_mouse_y;
		float m_mouse_dx, m_mouse_dy;
	};

	class LUHAME_API mouse_button_event :public event {
	public:
		inline int get_mouse_botton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		mouse_button_event(int button):
			m_button(button){ }

		int m_button;
	};

	class LUHAME_API mouse_button_pressed_event : mouse_button_event {
	public:
		mouse_button_pressed_event(int button,int repeat_count)
			:mouse_button_event(button),m_repeat_count(repeat_count){ }
		EVENT_CLASS_TYPE(MouseButtonPressed)
	private:
		int m_repeat_count;
	};

	class LUHAME_API mouse_button_released_event : public mouse_button_event
	{
	public:
		mouse_button_released_event(int button)
			: mouse_button_event(button) {
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}