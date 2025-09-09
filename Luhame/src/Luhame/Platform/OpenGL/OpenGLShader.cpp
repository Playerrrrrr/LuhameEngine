#include "pch.h"
#include "OpenGLShader.h"

#include <Glad/glad.h>
#include <sstream>
#include <limits>

namespace Luhame {


	GLenum opengl_shader::shader_type_from_string(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		return GL_NONE;
	}

	opengl_shader::opengl_shader(const std::string& filepath)
	{
		read_shader_from_file(filepath);
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(opengl_shader*),
			LH_RENDER_COMMAND_ARGS(this),
			([](opengl_shader* self)->void {
				self->compile_and_upload_shader();
			})
		)
	}

	void opengl_shader::bind()
	{


		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id),
			(
				[](renderer_id* l_renderer_id) {
					LH_CORE_INFO("use shader {}", *l_renderer_id);
					glUseProgram(*l_renderer_id);
				}
			)
		)
	}

	void opengl_shader::read_shader_from_file(const std::string& filepath)
	{
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			m_shader_source.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&m_shader_source[0], m_shader_source.size());
			in.close();
		}
		else
		{
			LH_CORE_WARN("Could not read shader file {0}", filepath);
		}
	}

	void opengl_shader::compile_and_upload_shader()
	{

		std::unordered_map<GLenum, std::string> shader_source;
			LH_INFO("{}", m_shader_source);

		const char* type_token = "#type";
		size_t typeTokenLength = strlen(type_token);

		size_t pos = m_shader_source.find(type_token, 0);
		while (pos != std::string::npos)
		{
			size_t eol = m_shader_source.find_first_of("\r\n", pos);

			LH_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = m_shader_source.substr(begin, eol - begin);
			LH_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified");


			size_t nextLinePos = m_shader_source.find_first_not_of("\r\n", eol);
			pos = m_shader_source.find(type_token, nextLinePos);
			shader_source[shader_type_from_string(type)] = m_shader_source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? m_shader_source.size() - 1 : nextLinePos));
		}

		std::vector<GLuint> shaderRendererIDs;


		GLuint program = glCreateProgram();
		for (auto& kv : shader_source)
		{
			GLenum type = kv.first;
			std::string& source = kv.second;

			GLuint shaderRendererID = glCreateShader(type);
			const GLchar* sourceCstr = (const GLchar*)source.c_str();
			glShaderSource(shaderRendererID, 1, &sourceCstr, 0);

			glCompileShader(shaderRendererID);

			GLint isCompiled = 0;
			glGetShaderiv(shaderRendererID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderRendererID, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderRendererID, maxLength, &maxLength, &infoLog[0]);

				LH_CORE_ERROR("Shader compilation failed:\n{0}", &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shaderRendererID);

				LH_CORE_ERROR(false, "Failed");
			}

			shaderRendererIDs.push_back(shaderRendererID);
			glAttachShader(program, shaderRendererID);
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			LH_CORE_ERROR("Shader compilation failed:\n{0}", &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : shaderRendererIDs)
				glDeleteShader(id);
		}

		// Always detach shaders after a successful link.
		for (auto id : shaderRendererIDs)
			glDetachShader(program, id);
		LH_CORE_INFO("create shader id {}", program);
		m_renderer_id = program;
	}


}