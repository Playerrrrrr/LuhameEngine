#include "pch.h"
#include "OpenGLTexture.h"

#include "Luhame/Renderer/RendererAPI.h"
#include "Luhame/Renderer/Renderer.h"

#include <Glad/glad.h>

namespace Luhame {

	static GLenum luhame_to_opengl_texture_format(texture_format format)
	{
		switch (format)
		{
		case Luhame::texture_format::RGB:     return GL_RGB;
		case Luhame::texture_format::RGBA:    return GL_RGBA;
		}
		return 0;
	}

	opengl_texture_2d::opengl_texture_2d(texture_format format, unsigned int width, unsigned int height)
		: m_format(format), m_width(width), m_height(height)
	{

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(
				renderer_id*, 
				texture_format,
				unsigned int,
				unsigned int),
			LH_RENDER_COMMAND_ARGS(
				&m_renderer_id, 
				m_format, 
				m_width, 
				m_height),
			([](renderer_id* l_renderer_id,texture_format l_texture_format,unsigned int l_width,unsigned int l_height) {
				glGenTextures(1, l_renderer_id);
				glBindTexture(GL_TEXTURE_2D, *l_renderer_id);
				glTexImage2D(
					GL_TEXTURE_2D, 0, 
					luhame_to_opengl_texture_format(l_texture_format), 
					l_width, l_height, 
					0, 
					luhame_to_opengl_texture_format(l_texture_format),
					GL_UNSIGNED_BYTE, nullptr
				);
				glBindTexture(GL_TEXTURE_2D, 0);
			})
		)
	}

	opengl_texture_2d::~opengl_texture_2d()
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id),
			(
				[](renderer_id* l_renderer_id) {
					glDeleteTextures(1, l_renderer_id);
				}
			)
		)
	}

}