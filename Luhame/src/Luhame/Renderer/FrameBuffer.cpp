#include"pch.h"
#include"Framebuffer.h"
#include"Luhame/Platform/OpenGL/OpenGLFramebuffer.h"
namespace Luhame {
	Luhame::framebuffer* framebuffer::create(uint32_t width, uint32_t height, framebuffer_format format)
	{
		Luhame::framebuffer* result = nullptr;

		switch (renderer_api::current())
		{
		case renderer_api_type::None:		return nullptr;
		case renderer_api_type::OpenGL:     result = new opengl_framebuffer(width, height, format);
		}
		framebuffer_pool::get_global()->add(result);
		return result;
	}

	framebuffer_pool* framebuffer_pool::s_instance = new framebuffer_pool;

	framebuffer_pool::framebuffer_pool(uint32_t max_fbs /* = 32 */)
	{

	}

	framebuffer_pool::~framebuffer_pool()
	{

	}

	std::weak_ptr<framebuffer> framebuffer_pool::allocate_buffer()
	{
		// m_pool.push_back();
		return std::weak_ptr<framebuffer>();
	}

	void framebuffer_pool::add(framebuffer* framebuffer)
	{
		m_pool.push_back(framebuffer);
	}
}