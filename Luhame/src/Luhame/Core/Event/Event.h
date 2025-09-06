#pragma once
#include<pch.h>
#include<Luhame/Core/Core.h>

namespace Luhame {
	enum class event_type {
		None = 0,
		WindowClosed, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved
	};

	enum event_category
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type)\
	static event_type get_static_type(){\
		return event_type::##type;\
	}\
	virtual event_type get_event_type()const override{return get_static_type();}\
	virtual const char* get_name()const override{return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int get_category_flags() const override {return category;}


	class LUHAME_API event {
		friend class event_dispatcher;
	public:
		virtual event_type get_event_type() const = 0;
		virtual const char* get_name() const = 0;
		virtual int  get_category_flags() const = 0;

		inline bool is_in_category(event_category category) {
			return get_category_flags() & category;
		}
	protected:
		bool m_handled = false;
	};

	class event_dispatcher {//处理事件的类，但是处理的函数需要给定
		template<class T>
		using EventFn = std::function<bool(T)>;
	public:
		event_dispatcher(event& e):m_event(e) {}

		template<class T>
		bool dispatch(EventFn<T> func) {
			if (m_event.get_event_type() == T::get_event_type()) {
				m_event.m_handled = func(*static_cast<T*>(&m_event));
				return true;
			}
			return false;
		}

	private:
		event& m_event;
	};

}



	