#pragma once

#include "Luhame/Renderer/IndexBuffer.h"

namespace Luhame {

	class LUHAME_API opengl_index_buffer : public index_buffer
	{
	public:
		opengl_index_buffer(unsigned int size);
		virtual ~opengl_index_buffer();

		virtual void set_data(void* buffer, unsigned int size, unsigned int offset = 0);
		virtual void bind() const;

		virtual unsigned int get_size() const { return m_size; }
		virtual renderer_id get_renderer_id() const { return m_renderer_id; }
	private:
		renderer_id m_renderer_id;
		unsigned int m_size;
	};

}
