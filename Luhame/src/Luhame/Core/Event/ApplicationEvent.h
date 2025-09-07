#pragma once
#include "Event.h"

namespace Luhame {

	class LUHAME_API window_resize_event : public event {
	public:
		window_resize_event(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {
		}
		inline unsigned int get_width() const { return m_Width; }
		inline unsigned int get_height() const { return m_Height; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "window resize event: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class LUHAME_API window_closed_event : public event
	{
	public:
		window_closed_event() {}

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class LUHAME_API app_tick_event : public event
	{
	public:
		app_tick_event() {}

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class LUHAME_API app_update_event : public event
	{
	public:
		app_update_event() {}

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class LUHAME_API app_render_event : public event
	{
	public:
		app_render_event() {}

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}