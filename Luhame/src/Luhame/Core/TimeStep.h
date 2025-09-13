#pragma once
namespace Luhame {
	class time_step
	{
	public:
		time_step();
		void update();
		float get_seconds()const  { return m_last_frame_time - m_now_time; }
		float get_milliseconds() const { return get_seconds() * 1000; }
	private:
		float m_now_time;
		float m_last_frame_time;
	};
}
