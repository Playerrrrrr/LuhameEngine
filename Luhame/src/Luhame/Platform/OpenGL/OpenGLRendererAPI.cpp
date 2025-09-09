#include "pch.h"
#include "Luhame/Renderer/RendererAPI.h"

#include <Glad/glad.h>

namespace Luhame {
	static unsigned int vao;
	void renderer_api::init() {
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		LH_CORE_INFO("vao id {}", vao);
	}

	void renderer_api::clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void renderer_api::set_clear_color(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void renderer_api::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void renderer_api::draw_indexed(unsigned int count) {

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);//Error
	}
}