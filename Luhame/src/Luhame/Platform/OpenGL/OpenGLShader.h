#pragma once

#include "Luhame/Renderer/Shader.h"
#include"glad/glad.h"
namespace Luhame {

	class LUHAME_API opengl_shader : public shader
	{
	public:


		opengl_shader(const std::string& filepath);

		virtual void bind() override;
	private:
		void read_shader_from_file(const std::string& filepath);
		void compile_and_upload_shader();

		static GLenum  shader_type_from_string(const std::string& type);


	private:
		renderer_id m_renderer_id;

		std::string m_shader_source;
	};

}