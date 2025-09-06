#pragma once
#include"Event.h"
namespace Luhame {

	class LUHAME_API key_event :public event {
	public:
		inline int get_key_code() const { return m_key_code; }
		EVENT_CLASS_CATEGORY(event_category::EventCategoryInput | event_category::EventCategoryKeyboard)
	protected:
		key_event(int key_code) :m_key_code(key_code) {}

		int m_key_code;
	};

	class LUHAME_API key_pressed_event :public key_event {
	public:
		key_pressed_event(int key_code,int repeat_count)
		:key_event(key_code), m_repeat_count(repeat_count){}
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_repeat_count;
	};

	class LUHAME_API key_released_event :public key_event {
	public:
		key_released_event(int key_code)
			:key_event(key_code){ }
		EVENT_CLASS_TYPE(KeyReleased)
	};

}