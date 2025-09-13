#include "pch.h"
#include "OpenGLBuffer.h"

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
					glGenBuffers(1, l_renderer_id);
					if constexpr(LH_RENDERER_DEBUG)
					LH_CORE_INFO("gen index{}", *l_renderer_id);
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
		m_size = size;
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*, void*, unsigned int, unsigned int),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id, buffer, size, offset),
			(
				[](renderer_id* rd_id, void* l_buffer, unsigned int l_size, unsigned int l_offset) {
			LH_CORE_INFO("index id :{0} {1} {2} {3}", *rd_id, l_size, *(int*)l_buffer, *((int*)l_buffer + l_size / 4 - 1));
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
			if constexpr (LH_RENDERER_DEBUG)
			LH_CORE_INFO("bind index {}", *l_renderer_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *l_renderer_id);
		}
				)
		)
	}


	opengl_vertex_buffer::opengl_vertex_buffer(unsigned int size)
		: m_renderer_id(0), m_size(size)
	{

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id),
			([](renderer_id* m_renderer_id)->void {
				glGenBuffers(1, m_renderer_id);
				if constexpr (LH_RENDERER_DEBUG)
				LH_CORE_INFO("gen vertex{}", *m_renderer_id);
			})
		)
	}

	opengl_vertex_buffer::~opengl_vertex_buffer()
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id),
			LH_RENDER_COMMAND_ARGS(m_renderer_id),
			([](renderer_id l_renderer_id)->void {
				glDeleteBuffers(1, &l_renderer_id);
			})
		)
	}

	void opengl_vertex_buffer::set_data(void* buffer, unsigned int size, unsigned int offset)
	{
		m_size = size;
		//这里要传renderer_id！！！！！！
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*, void*, unsigned int, unsigned int),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id, buffer, size, offset),
			([](renderer_id* l_renderer_id, void* l_buffer, unsigned int l_size, unsigned int l_offset)->void {
			if constexpr (LH_RENDERER_DEBUG)
			LH_CORE_INFO("vertex id :{0} {1} {2} {3}", *l_renderer_id, l_size, *(float*)l_buffer, *((float*)l_buffer + l_size / 4 - 1));

			glBindBuffer(GL_ARRAY_BUFFER, *l_renderer_id);
			glBufferData(GL_ARRAY_BUFFER, l_size, l_buffer, GL_STATIC_DRAW);
		})
		)
	}

	void opengl_vertex_buffer::bind() const
	{

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(const renderer_id*),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id),
			([](const renderer_id* m_renderer_id)->void {
			if constexpr (LH_RENDERER_DEBUG)
			LH_CORE_INFO("bind vertex {}",*m_renderer_id);

			glBindBuffer(GL_ARRAY_BUFFER, *m_renderer_id);			
			// TODO: Extremely temp, by default provide positions and texcoord attributes
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)(3 * sizeof(float)));
		})
		)
	}



}