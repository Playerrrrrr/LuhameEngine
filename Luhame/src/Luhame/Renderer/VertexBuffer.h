#pragma once
#include"Luhame/Core/Core.h"
#include "Luhame/Renderer/Renderer.h"

namespace Luhame {

	class LUHAME_API vertex_buffer
	{
	public:
		virtual ~vertex_buffer() {}

		virtual void set_data(void* buffer, unsigned int size, unsigned int offset = 0) = 0;
		virtual void bind() const = 0;

		virtual unsigned int get_size() const = 0;
		virtual renderer_id get_renderer_id() const = 0;

		static vertex_buffer* create(unsigned int size = 0);
	};

}