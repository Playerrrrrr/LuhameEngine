#include "pch.h"
#include "Shader.h"

#include "Luhame/Platform/OpenGL/OpenGLShader.h"

namespace Luhame {

	shader* shader::create(const std::string& filepath)
	{
		shader* result;
		switch (renderer_api::current())
		{
		case renderer_api_type::None: result = nullptr;
		case renderer_api_type::OpenGL : result = new opengl_shader(filepath);
		}
		s_all_shaders[filepath] = result;
		return result;
	}

}