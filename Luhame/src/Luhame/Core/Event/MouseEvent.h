#pragma once
#include"Event.h"

namespace Luhame {

	class LUHAME_API mouse_moved_event : public event {
	public:
		mouse_moved_event(float x,float y)
		:m_mouse_x(x),m_mouse_y(y){}

		inline float get_x() const { return m_mouse_x; }
		inline float get_y() const { return m_mouse_y; }

		std::string to_string() const override {
			std::stringstream ss;
			ss << "mouse move event: " << m_mouse_x << " " << m_mouse_y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse| EventCategoryInput)

	private:
		float m_mouse_x, m_mouse_y;
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
		mouse_button_pressed_event(int button)
			:mouse_button_event(button){ }
		EVENT_CLASS_TYPE(MouseButtonPressed)


		std::string to_string() const override {
			return "mouse button pressed evnet";
		}
	private:
	};

	class LUHAME_API mouse_button_released_event : public mouse_button_event
	{
	public:
		mouse_button_released_event(int button)
			: mouse_button_event(button) {
		}
		std::string to_string() const override {
			return "mouse button release evnet";
		}
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class LUHAME_API mouse_scrolled_event : public event
	{
	public:
		mouse_scrolled_event(float xOffset, float yOffset)
			: m_x_offset(xOffset), m_y_Offset(yOffset) {
		}

		inline float get_xoffset() const { return m_x_offset; }
		inline float get_yoffset() const { return m_y_Offset; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << get_xoffset() << ", " << get_yoffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_x_offset, m_y_Offset;
	};
}