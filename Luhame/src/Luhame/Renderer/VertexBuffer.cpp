#include "pch.h"

#include "Luhame/Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Luhame {

	vertex_buffer* vertex_buffer::create(unsigned int size)
	{
		switch (renderer_api::current())
		{
			case renderer_api_type::None:    return nullptr;
			case renderer_api_type::OpenGL:  return new opengl_vertex_buffer(size);
		}
		return nullptr;
	}

}