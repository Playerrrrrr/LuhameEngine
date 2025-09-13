#include "pch.h"
#include "TimeStep.h"
#include"GLFW/glfw3.h"
Luhame::time_step::time_step()
{
	m_now_time = (float)glfwGetTime();
	m_last_frame_time = m_now_time;
}
void Luhame::time_step::update()
{
	m_last_frame_time = m_now_time;
	m_now_time = (float)glfwGetTime();
}
