#pragma once
#include"Luhame/Core/Core.h"
#include"Luhame/Core/Event/Event.h"
namespace Luhame {
	struct window_props//创建window的初始化参数
	{
		std::string m_title;
		uint32_t m_width;
		uint32_t m_height;


		window_props(
			const std::string& title,
			uint32_t width = 1280,
			uint32_t height = 720
		) 
		:m_title(title),m_width(width),m_height(height){
		}
	};

	class LUHAME_API window{
	public:
		using event_callback_fn = std::function<void(event&)>;
		virtual ~window() {}
		virtual void on_update() = 0;
		virtual void set_event_callback(const event_callback_fn& callback) = 0;
		virtual unsigned int get_width() const = 0;
		virtual unsigned int get_height() const = 0;

		static window* create(const window_props& props);
	protected:

	};
}

