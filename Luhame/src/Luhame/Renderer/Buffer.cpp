#include "pch.h"
#include"Buffer.h"
#include "Luhame/Platform/OpenGL/OpenGLBuffer.h"

namespace Luhame {
	index_buffer* index_buffer::create(unsigned int size)
	{
		switch (renderer_api::current())
		{
		case renderer_api_type::None:    return nullptr;
		case renderer_api_type::OpenGL:  return new opengl_index_buffer(size);
		}
		return nullptr;
	}

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