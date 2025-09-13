#include "pch.h"
#include "Texture.h"

#include "Luhame/Renderer/RendererAPI.h"
#include "Luhame/Platform/OpenGL/OpenGLTexture.h"

namespace Luhame {

	texture_2d* texture_2d::create(texture_format format, unsigned int width, unsigned int height)
	{
		switch (renderer_api::current())
		{
		case renderer_api_type::None: return nullptr;
		case renderer_api_type::OpenGL: return new opengl_texture_2d(format, width, height);
		}
		return nullptr;
	}

	texture_2d* texture_2d::create(const std::string& file_path, bool srgb)
	{
		switch (renderer_api::current())
		{
		case renderer_api_type::None: return nullptr;
		case renderer_api_type::OpenGL: return new opengl_texture_2d(file_path, srgb);
		}
		return nullptr;
	}

	texture_cube* texture_cube::create(const std::string& file_path) {
		switch (renderer_api::current())
		{
		case renderer_api_type::None: return nullptr;
		case renderer_api_type::OpenGL: return new opengl_texture_cube(file_path);
		}
		return nullptr;
	}

}