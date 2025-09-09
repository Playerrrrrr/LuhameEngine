#include "pch.h"

#include "Luhame/Platform/OpenGL/OpenGLIndexBuffer.h"

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

}