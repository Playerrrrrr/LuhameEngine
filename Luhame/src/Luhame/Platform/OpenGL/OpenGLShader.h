#pragma once

#include "Luhame/Renderer/Shader.h"
#include"glad/glad.h"
namespace Luhame {
	//解析xxxx.h文件之前解析xxxx.cpp
	class LUHAME_API opengl_shader : public shader
	{
	public:
		virtual void reload() override;
		virtual void set_float(const std::string& name, float value) override;
		virtual void set_mat4(const std::string& name, const glm::mat4& value) override;
		virtual void set_int(const std::string& name, int value) override;
		
		virtual const std::string& get_name() const override { return m_name; }
		virtual void upload_uniform_buffer(const uniform_buffer_base&) override;
		opengl_shader(const std::string& filepath);

		virtual void bind() override;
	private:
		void read_shader_from_file(const std::string& filepath);
		void compile_and_upload_shader();

		static GLenum  shader_type_from_string(const std::string& type);

		void upload_uniform_int(const std::string& name, const int& value);
		void upload_uniform_float(const std::string& name, const float& value);
		void upload_uniform_float2(const std::string& name, const glm::vec2& values);
		void upload_uniform_float3(const std::string& name, const glm::vec3& values);
		void upload_uniform_float4(const std::string& name, const glm::vec4& values);
		void upload_uniform_mat4(const std::string& name, const glm::mat4& values);
	private:
		renderer_id m_renderer_id;
		std::string m_shader_source;
		std::string m_name, m_asset_path;
	};

}