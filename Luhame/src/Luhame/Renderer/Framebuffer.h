#pragma once
#include"Luhame/Renderer/RendererAPI.h"

namespace Luhame {
	enum class framebuffer_format {
		None = 0,
		RGBA8,
		RGBA16F
	};

	class framebuffer
	{
	public:
		static framebuffer* create(uint32_t width, uint32_t height, framebuffer_format format);

		virtual ~framebuffer() {}
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual void bind_texture(uint32_t slot = 0) const = 0;

		virtual renderer_id get_renderer_id() const = 0;
		virtual renderer_id get_color_attachment_renderer_id() const = 0;
		virtual renderer_id get_depth_attachment_renderer_id() const = 0;
	};

	class framebuffer_pool final
	{
	public:
		framebuffer_pool(uint32_t max_fbs = 32);
		~framebuffer_pool();

		std::weak_ptr<framebuffer> allocate_buffer();
		void add(framebuffer* framebuffer);

		const std::vector<framebuffer*>& get_all() const { return m_pool; }

		inline static framebuffer_pool* get_global() { return s_instance; }
	private:
		std::vector<framebuffer*> m_pool;

		static framebuffer_pool* s_instance;
	};
}