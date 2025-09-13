#include "pch.h"
#include "RenderCommandQueue.h"
#include"imgui.h"
#define LH_RENDER_TRACE(...) LH_CORE_TRACE(__VA_ARGS__)



namespace Luhame {
	render_command_queue::render_command_queue()
	{
		m_command_buffer = new unsigned char[100 * 1024 * 1024]; // 10mb buffer
		m_command_buffer_ptr = m_command_buffer;
		memset(m_command_buffer, 0, 10 * 1024 * 1024);
	}

	render_command_queue::~render_command_queue()
	{
		delete[] m_command_buffer;
	}

	void* render_command_queue::allocate(render_command_fn func, command_para_size size)
	{
		*(render_command_fn*)m_command_buffer_ptr = func;
		m_command_buffer_ptr += sizeof render_command_fn;
		*(command_para_size*)m_command_buffer_ptr = size;
		m_command_buffer_ptr += sizeof(int);
		void* memory = m_command_buffer_ptr;
		m_command_buffer_ptr += size;
		m_render_command_count++;
		return memory;
	}


	void render_command_queue::execute()
	{
		LH_CORE_INFO("render_command_queue::execute -- {0} commands, {1} bytes", m_render_command_count, m_command_buffer_ptr - m_command_buffer);

		byte* buffer = m_command_buffer;

		for (int i = 0; i < m_render_command_count; i++)
		{
			render_command_fn fn = *(render_command_fn*)buffer;
			buffer += sizeof(render_command_fn);
			command_para_size size = *(command_para_size*)buffer;
			buffer += sizeof(command_para_size);
			assert(ImGui::GetCurrentContext());
			(*fn)(buffer);
			buffer += size;
		}
		m_command_buffer_ptr = m_command_buffer;
		m_render_command_count = 0;
	}
}