#pragma once
#include"Luhame/Renderer/Framebuffer.h"
namespace Luhame {
	using uint32_t = unsigned int;


	class opengl_framebuffer : public framebuffer
	{
	public:
		opengl_framebuffer(uint32_t width, uint32_t height, framebuffer_format format);
		virtual ~opengl_framebuffer();

		virtual void resize(uint32_t width, uint32_t height) override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void bind_texture(uint32_t slot = 0) const override;

		virtual renderer_id get_renderer_id() const { return m_renderer_id; }
		virtual renderer_id get_color_attachment_renderer_id() const { return m_color_attachment; }
		virtual renderer_id get_depth_attachment_renderer_id() const { return m_depth_attachment; }

		virtual uint32_t get_width() const { return m_width; }
		virtual uint32_t get_height() const { return m_height; }
		virtual framebuffer_format get_format() const { return m_format; }
	private:
		renderer_id m_renderer_id = 0;
		uint32_t m_width, m_height;
		framebuffer_format m_format;
		bool has_init = false;
		renderer_id m_color_attachment, m_depth_attachment;
	};
}