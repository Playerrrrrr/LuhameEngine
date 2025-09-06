#include"pch.h"
#include "WindowsWindow.h"
#include"Luhame/Core/Log.h"

namespace Luhame {
	window* window::create(const window_props& props) {
		return  new windows_window(props);
	}

	windows_window::windows_window(const window_props& props)
	{
		init(props);
	}
	windows_window::~windows_window()
	{
	}
	void windows_window::on_update()
	{
		LH_CORE_INFO("window update");
	}
	void windows_window::init(const window_props& props)
	{
		m_height = props.m_height;
		m_width = props.m_width;
		m_title = props.m_title;
	}
	void windows_window::shut_down()
	{
	}
}