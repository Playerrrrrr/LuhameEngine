#include "pch.h"
#include "Shader.h"

#include "Luhame/Platform/OpenGL/OpenGLShader.h"

namespace Luhame {

	shader* shader::create(const std::string& filepath)
	{
		switch (renderer_api::current())
		{
		case renderer_api_type::None: return nullptr;
		case renderer_api_type::OpenGL : return new opengl_shader(filepath);
		}
		return nullptr;
	}

}