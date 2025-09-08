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

			inline int get_repeat_count() { return m_repeat_count; }

		std::string to_string()const override {
			std::stringstream ss;
			ss << "key pressed event: " << m_key_code << " (" << m_repeat_count << " repeats)";
			return ss.str();
		}

	private:
		int m_repeat_count;
	};

	class LUHAME_API key_released_event :public key_event {
	public:
		key_released_event(int key_code)
			:key_event(key_code){ }
		EVENT_CLASS_TYPE(KeyReleased)

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "key released event: " << m_key_code;
			return ss.str();
		}
	};

	class key_typed_event : public event
	{
	public:
		key_typed_event(unsigned int keycode)
			: m_keycode(keycode) {
		}

		unsigned int get_key_code() const { return m_keycode; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "key_typed_event: " << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
			EVENT_CLASS_CATEGORY(event_category::EventCategoryInput | event_category::EventCategoryKeyboard)
	private:
		unsigned int m_keycode;
	};

}