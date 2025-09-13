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



	opengl_shader::opengl_shader(const std::string& file_path)
	:m_asset_path(file_path){
		size_t found = file_path.find_last_of("/\\");
		m_name = found != std::string::npos ? file_path.substr(found + 1) : file_path;
		reload();

	}

	void opengl_shader::reload()
	{
		read_shader_from_file(m_asset_path);
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
			if constexpr (LH_RENDERER_DEBUG)

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
		LH_INFO("creating shader : {}", m_name);

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


		// Bind default texture unit
		upload_uniform_int("u_Texture",0);

		// PBR shader textures
		upload_uniform_int("u_AlbedoTexture", 1);
		upload_uniform_int("u_NormalTexture", 2);
		upload_uniform_int("u_MetalnessTexture", 3);
		upload_uniform_int("u_RoughnessTexture", 4);

		upload_uniform_int("u_EnvRadianceTex", 10);
		upload_uniform_int("u_EnvIrradianceTex", 11);

		upload_uniform_int("u_BRDFLUTTexture", 15);
	}

	void opengl_shader::set_float(const std::string& name, float value) {
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(opengl_shader*, std::string,float),
			LH_RENDER_COMMAND_ARGS(this, name ,value),
			([](opengl_shader* self, std::string name,float value) {
				self->upload_uniform_float(name, value);
			})
		)
	}

	void opengl_shader::set_mat4(const std::string& name, const glm::mat4& value) {
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(opengl_shader*, std::string, glm::mat4),
			LH_RENDER_COMMAND_ARGS(this, name, value),
			([](opengl_shader* self, std::string name, glm::mat4 value) {
				self->upload_uniform_mat4(name, value);
			})
		)
	}

	void opengl_shader::set_int(const std::string& name, int value)
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(opengl_shader*, std::string, int),
			LH_RENDER_COMMAND_ARGS(this, name, value),
			([](opengl_shader* self, std::string name, int value) {
			self->upload_uniform_int(name, value);
		})
		)
	}

	void opengl_shader::upload_uniform_buffer(const uniform_buffer_base& ufb)
	{
		for (int i = 0; i < ufb.get_uniform_count(); i++) {
			const uniform_decl& t_decl = ufb.get_uniforms()[i];
			switch (t_decl.type) {
				case uniform_type::int32: {
					std::string name = t_decl.name;
					int value = *(int*)(ufb.get_buffer() + t_decl.offset);
					LH_RENDER_COMMAND_SUBMIT(
						LH_RENDER_COMMAND_TYPES(opengl_shader*, std::string, int),
						LH_RENDER_COMMAND_ARGS(this, t_decl.name, value),
						([](opengl_shader* self, std::string name, int value) {
						self->upload_uniform_int(name, value);
					})
					)
				}
				case uniform_type::float1 :{
					std::string name = t_decl.name;
					float value = *(float*)(ufb.get_buffer() + t_decl.offset);
					LH_RENDER_COMMAND_SUBMIT(
						LH_RENDER_COMMAND_TYPES(opengl_shader*,std::string,float),
						LH_RENDER_COMMAND_ARGS(this,t_decl.name,value),
						([](opengl_shader* self,std::string name, float value) {
							self->upload_uniform_float(name, value);
						})
					)
				}
				case uniform_type::float2: {
					std::string name = t_decl.name;
					glm::vec2 value = *(glm::vec2*)(ufb.get_buffer() + t_decl.offset);
					LH_RENDER_COMMAND_SUBMIT(
						LH_RENDER_COMMAND_TYPES(opengl_shader*, std::string, glm::vec2),
						LH_RENDER_COMMAND_ARGS(this, t_decl.name, value),
						([](opengl_shader* self, std::string name, glm::vec2 value) {
							self->upload_uniform_float2(name, value);
						})
					)
				}
				case uniform_type::float3: {
					std::string name = t_decl.name;
					glm::vec3 value = *(glm::vec3*)(ufb.get_buffer() + t_decl.offset);
					LH_RENDER_COMMAND_SUBMIT(
						LH_RENDER_COMMAND_TYPES(opengl_shader*, std::string, glm::vec3),
						LH_RENDER_COMMAND_ARGS(this, t_decl.name, value),
						([](opengl_shader* self, std::string name, glm::vec3 value) {
							self->upload_uniform_float3(name, value);
						})
					)
				}
				case uniform_type::float4: {
					const std::string name = t_decl.name;
					glm::vec4 value = *(glm::vec4*)(ufb.get_buffer() + t_decl.offset);
					LH_RENDER_COMMAND_SUBMIT(
						LH_RENDER_COMMAND_TYPES(opengl_shader*, std::string, glm::vec4),
						LH_RENDER_COMMAND_ARGS(this, t_decl.name, value),
						([](opengl_shader* self, std::string name, glm::vec4 value) {
						self->upload_uniform_float4(name, value);
					})
					)
				}
				case uniform_type::matrix4x4: {
					const std::string name = t_decl.name;
					glm::mat4 value = *(glm::mat4*)(ufb.get_buffer() + t_decl.offset);
					LH_RENDER_COMMAND_SUBMIT(
						LH_RENDER_COMMAND_TYPES(opengl_shader*, std::string, glm::mat4),
						LH_RENDER_COMMAND_ARGS(this, t_decl.name, value),
						([](opengl_shader* self, std::string name, glm::mat4 value) {
						self->upload_uniform_mat4(name, value);
					})
					)
				}
			}
		}
	}

	void opengl_shader::upload_uniform_int(const std::string& name, const int& value)
	{
		//绑定shader程序
		glUseProgram(m_renderer_id);
		//寻找名为name的uniform所在位置
		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		if (location != -1)
			//加载数据到shader中
			glUniform1i(location, value);
		else
			LH_CORE_ERROR("Uniform '{0}' not found!", name);
	}

	void opengl_shader::upload_uniform_float(const std::string& name,const float& value)
	{
		glUseProgram(m_renderer_id);
		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		if (location != -1)
			//加载数据到shader中
			glUniform1f(location, value);
		else
			LH_CORE_ERROR("Uniform '{0}' not found!", name);
	}
	void opengl_shader::upload_uniform_float2(const std::string& name, const glm::vec2& values)
	{
		glUseProgram(m_renderer_id);
		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		if (location != -1)
			//加载数据到shader中
			glUniform2f(location, values.x, values.y);
		else
			LH_CORE_ERROR("Uniform '{0}' not found!", name);
	}
	void opengl_shader::upload_uniform_float3(const std::string& name, const glm::vec3& values)
	{
		glUseProgram(m_renderer_id);
		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		if (location != -1)
			//加载数据到shader中
			glUniform3f(location, values.x, values.y, values.z);
		else
			LH_CORE_ERROR("Uniform '{0}' not found!", name);
	}
	void opengl_shader::upload_uniform_float4(const std::string& name, const glm::vec4& values)
	{
		glUseProgram(m_renderer_id);
		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		if (location != -1)
			//加载数据到shader中
			glUniform4f(location, values.x, values.y, values.z, values.w);
		else
			LH_CORE_ERROR("Uniform '{0}' not found!", name);
	}

	void opengl_shader::upload_uniform_mat4(const std::string& name, const glm::mat4& values)
	{
		glUseProgram(m_renderer_id);
		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, (const float*)&values);
		else
			LH_CORE_ERROR("Uniform '{0}' not found!", name);
	}


}