#pragma once

#include "Luhame/Renderer/RendererAPI.h"
#include "Luhame/Renderer/Texture.h"

namespace Luhame {

	class LUHAME_API opengl_texture_2d : public texture_2d
	{
	public:
		opengl_texture_2d(texture_format format, unsigned int width, unsigned int height);
		~opengl_texture_2d();

		virtual texture_format get_format() const { return m_format; }
		virtual unsigned int get_width() const { return m_width; }
		virtual unsigned int get_height() const { return m_height; }
	private:
		renderer_id m_renderer_id;
		texture_format m_format;
		unsigned int m_width, m_height;
	};
}