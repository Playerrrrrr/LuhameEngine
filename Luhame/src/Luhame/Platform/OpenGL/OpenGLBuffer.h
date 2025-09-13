#pragma once
#pragma once

#include "Luhame/Renderer/Buffer.h"

namespace Luhame {

	class LUHAME_API opengl_index_buffer : public index_buffer
	{
	public:
		opengl_index_buffer(unsigned int size);
		virtual ~opengl_index_buffer();

		virtual void set_data(void* buffer, unsigned int size, unsigned int offset = 0);
		virtual void bind() const;
		virtual uint32_t get_count() const override { return m_size / sizeof(uint32_t); };
		virtual unsigned int get_size() const { return m_size; }
		virtual renderer_id get_renderer_id() const { return m_renderer_id; }
	private:
		renderer_id m_renderer_id;
		unsigned int m_size;
	};


	class LUHAME_API opengl_vertex_buffer : public vertex_buffer
	{
	public:
		opengl_vertex_buffer(unsigned int size);
		virtual ~opengl_vertex_buffer();

		virtual void set_data(void* buffer, unsigned int size, unsigned int offset = 0);
		virtual void bind() const;

		virtual unsigned int get_size() const { return m_size; }
		virtual renderer_id get_renderer_id() const { return m_renderer_id; }

	private:
		renderer_id m_renderer_id;
		unsigned int m_size;
	};
}