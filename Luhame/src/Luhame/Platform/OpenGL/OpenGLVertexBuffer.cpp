#include "pch.h"
#include "OpenGLVertexBuffer.h"

#include <Glad/glad.h>

namespace Luhame {


	opengl_vertex_buffer::opengl_vertex_buffer(unsigned int size)
		: m_renderer_id(0), m_size(size)
	{

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id),
			([](renderer_id* m_renderer_id)->void {
			LH_CORE_INFO("gen vertex");
			glGenBuffers(1, m_renderer_id);
			})
		)
	}

	opengl_vertex_buffer::~opengl_vertex_buffer()
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id),
			([](renderer_id* l_renderer_id)->void {
			glDeleteBuffers(1, l_renderer_id);
		})
		)
	}

	void opengl_vertex_buffer::set_data(void* buffer, unsigned int size, unsigned int offset)
	{
		//这里要传renderer_id！！！！！！
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*,void*,unsigned int,unsigned int),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id,buffer, size,offset),
			([](renderer_id* l_renderer_id,void* l_buffer, unsigned int l_size, unsigned int l_offset)->void {
			LH_CORE_INFO("vertex id :{0} {1} {2} {3}", *l_renderer_id, l_size, *(float*)l_buffer, *((float*)l_buffer+ l_size/4-1));
			glBindBuffer(GL_ARRAY_BUFFER, *l_renderer_id);
			glBufferData(GL_ARRAY_BUFFER, l_size, l_buffer, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
			glEnableVertexAttribArray(0);
		})
		)
	}

	void opengl_vertex_buffer::bind() const
	{

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id),
			LH_RENDER_COMMAND_ARGS(m_renderer_id),
			([](renderer_id m_renderer_id)->void {
			LH_CORE_INFO("bind vertex");
			glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		})
		)
	}

}