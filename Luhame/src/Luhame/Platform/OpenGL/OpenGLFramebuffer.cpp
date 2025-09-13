#include "pch.h"
#include"Luhame/Platform/OpenGL/OpenGLFramebuffer.h"
#include "Luhame/Renderer/Renderer.h"
#include <glad/glad.h>
namespace Luhame {
	opengl_framebuffer::opengl_framebuffer(uint32_t width, uint32_t height, framebuffer_format format)
		: m_width(width), m_height(height), m_format(format)
	{
		resize(width, height);
		has_init = true;
	}

	opengl_framebuffer::~opengl_framebuffer()
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id),
			LH_RENDER_COMMAND_ARGS(this->m_renderer_id),
			(
				[](renderer_id rd_id) {
					glDeleteFramebuffers(1, &rd_id);
				}
			)
		)
	}

	void opengl_framebuffer::resize(uint32_t width, uint32_t height)
	{
		if (m_width == width && m_height == height&& has_init)
			return;

		m_width = width;
		m_height = height;

		LH_RENDER_COMMAND_SUBMIT_SELF(
			[](opengl_framebuffer* self) {
				if (self->m_renderer_id)
				{
					LH_CORE_ERROR("trying to allocate a framebuffer before the old framebuffer has not be deleted!");
				}
				glGenFramebuffers(1, &self->m_renderer_id);
				glBindFramebuffer(GL_FRAMEBUFFER, self->m_renderer_id);

				LH_CORE_ERROR("gen frame buffer : {}", self->m_renderer_id);

				glGenTextures(1, &self->m_color_attachment);
				glBindTexture(GL_TEXTURE_2D, self->m_color_attachment);

				// TODO: Create Luhame texture object based on format here
				if (self->m_format == framebuffer_format::RGBA16F)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, self->m_width, self->m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
				}
				else if (self->m_format == framebuffer_format::RGBA8)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self->m_width, self->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				}
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->m_color_attachment, 0);

				glGenTextures(1, &self->m_depth_attachment);
				glBindTexture(GL_TEXTURE_2D, self->m_depth_attachment);
				glTexImage2D(
					GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, self->m_width, self->m_height, 0,
					GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
				);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, self->m_depth_attachment, 0);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
					LH_CORE_ERROR("Framebuffer is incomplete!");

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		)
	}

	void opengl_framebuffer::bind() const
	{
		LH_RENDER_COMMAND_SUBMIT_SELF(
			(
				[](const opengl_framebuffer* self) {

			if constexpr (LH_RENDERER_DEBUG)
					LH_CORE_INFO("bind to framebuffer : {}", self->m_renderer_id);

					glBindFramebuffer(GL_FRAMEBUFFER, self->m_renderer_id);
					glViewport(0, 0, self->m_width, self->m_height);
				}
			)
		)
	}

	void opengl_framebuffer::unbind() const
	{
		LH_RENDER_COMMAND_SUBMIT_SELF(
			(
				[](const opengl_framebuffer* self) {
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
			)
		)
	}

	void opengl_framebuffer::bind_texture(uint32_t slot) const
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(const opengl_framebuffer*,uint32_t),
			LH_RENDER_COMMAND_ARGS(this,slot),
			[](const opengl_framebuffer* self, uint32_t slot) {
				glActiveTexture(GL_TEXTURE0 + slot);
				glBindTexture(GL_TEXTURE_2D, self->m_color_attachment);
				if constexpr (LH_RENDERER_DEBUG)
				LH_CORE_INFO("bind frame buffer as texture : {}", self->m_color_attachment);
			}
		)

	}
}