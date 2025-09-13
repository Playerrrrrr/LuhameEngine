#pragma once

#include "Luhame/Renderer/RendererAPI.h"
#include "Luhame/Renderer/Texture.h"

#include<string>

namespace Luhame {

	class LUHAME_API opengl_texture_2d : public texture_2d
	{
	public:
		opengl_texture_2d(texture_format format, unsigned int width, unsigned int height);
		~opengl_texture_2d();
		opengl_texture_2d(const std::string& file_name, bool srgb);
		virtual void bind(uint32_t) const override;
		virtual texture_format get_format() const { return m_format; }
		virtual unsigned int get_width() const { return m_width; }
		virtual unsigned int get_height() const { return m_height; }
		virtual const std::string& get_path() const override { return m_file_path; }
		virtual renderer_id get_renderer_id() const  { return m_renderer_id; }
		//virtual renderer_id get_renderer_id() const override { return m_renderer_id; }
		virtual void release() override;
	private:
		renderer_id m_renderer_id;
		texture_format m_format;
		unsigned int m_width, m_height;
		std::string m_file_path;
		unsigned char* m_image_data;
		bool has_state = false;//表示实例中是否有opengl对象存储
	};

	class opengl_texture_cube : public texture_cube
	{
	public:
		opengl_texture_cube(const std::string& path);
		virtual ~opengl_texture_cube();

		virtual void bind(uint32_t slot = 0) const;

		virtual texture_format get_format() const { return m_format; }
		virtual unsigned int get_width() const { return m_width; }
		virtual unsigned int get_height() const { return m_height; }
		virtual const std::string& get_path() const override { return m_file_path; }
		virtual renderer_id get_renderer_id() const  { return m_renderer_id; }
		virtual void release();
	private:
		renderer_id m_renderer_id;
		texture_format m_format;
		unsigned int m_width, m_height;

		unsigned char* m_image_data;

		std::string m_file_path;
		bool has_state = false;//表示实例中是否有opengl对象存储

	};
}