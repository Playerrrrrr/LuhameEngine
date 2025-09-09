#include "pch.h"
#include "OpenGLIndexBuffer.h"

#include <Glad/glad.h>

namespace Luhame {

	opengl_index_buffer::opengl_index_buffer(unsigned int size)
		: m_renderer_id(0), m_size(size)
	{

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id),
			(
				[](renderer_id* l_renderer_id) {
					LH_CORE_INFO("gen index");
					glGenBuffers(1, l_renderer_id);
				}
			)
		)
	}

	opengl_index_buffer::~opengl_index_buffer()
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id),
			(
				[](renderer_id* l_renderer_id) {
					glDeleteBuffers(1, l_renderer_id);
				}
			)
		)
	}

	void opengl_index_buffer::set_data(void* buffer, unsigned int size, unsigned int offset)
	{

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*,void*,unsigned int,unsigned int),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id,buffer,size,offset),
			(
				[](renderer_id* rd_id,void* l_buffer, unsigned int l_size, unsigned int l_offset) {
					LH_CORE_INFO("index id :{0} {1} {2} {3}", *rd_id, l_size, *(int*)l_buffer, *((int*)l_buffer + l_size/4 -1));
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *rd_id);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, l_size, l_buffer, GL_STATIC_DRAW);
				}
			)
		)
	}

	void opengl_index_buffer::bind() const
	{

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(const renderer_id*),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id),
			(
				[](const renderer_id* l_renderer_id) {
					LH_CORE_INFO("bind index");
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *l_renderer_id);
				}
			)
		)
	}

}