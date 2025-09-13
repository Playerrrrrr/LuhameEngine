#include "pch.h"
#include "Luhame/Renderer/RendererAPI.h"

#include <Glad/glad.h>

namespace Luhame {

	static void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
			LH_CORE_ERROR("{0}", message);
	}
	unsigned int vao;
	void renderer_api::init() {
		glDebugMessageCallback(OpenGLLogMessage, nullptr);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glFrontFace(GL_CCW);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		auto& caps = renderer_api::get_capabilities();

		caps.vendor = (const char*)glGetString(GL_VENDOR);
		caps.renderer = (const char*)glGetString(GL_RENDERER);
		caps.version = (const char*)glGetString(GL_VERSION);
		LH_CORE_INFO("opengl version {}: ", caps.version);
		glGetIntegerv(GL_MAX_SAMPLES, &caps.max_samples);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &caps.max_anisotropy);
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
	void renderer_api::draw_indexed(unsigned int count,draw_config config) {
		if (config.depth_test)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);//Error
	}
}