#include "pch.h"
#include "Renderer.h"
#include"RenderCommand.h"
#include<tuple>
#include<glad/glad.h>
#include"Luhame/Renderer/Buffer.h"
namespace Luhame {

	renderer* renderer::s_instance = new renderer();
	renderer_api_type renderer_api::s_current_renderer_api = renderer_api_type::OpenGL;
	void renderer::init()
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(),
			LH_RENDER_COMMAND_ARGS(),
			(
				[](){
					renderer_api::init();
				}
			)
		)
	}

	void renderer::clear()
	{
		// HZ_RENDER(clear());
	}

	void renderer::clear(float r, float g, float b, float a)
	{
		float params[4] = { r, g, b, a };

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(float,float, float,float),
			LH_RENDER_COMMAND_ARGS(r, g, b, a),
			(
			[](float r, float g, float b, float a) -> void {
			LH_CORE_INFO("CLEAN");
				renderer_api::clear(r, g, b, a);
			}
			)
		)
	}

	void renderer::draw_indexed(unsigned int count,draw_config config)
	{

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(unsigned int, draw_config),
			LH_RENDER_COMMAND_ARGS(count, config),
			(
				[](unsigned int count, draw_config config) -> void {
				renderer_api::draw_indexed(count, config);
			}
			)
		)
	}

	void renderer::clear_magenta()
	{
		clear(1, 0, 1);
	}

	void renderer::set_clear_color(float r, float g, float b, float a)
	{
		// HZ_RENDER(set_clear_color(r, g, b, a));
	}

	void renderer::wait_and_render()
	{
		s_instance->m_command_queue.execute();
	}
}

#undef LH_RENDER_COMMAND_SUBMIT
#undef LH_RENDER_COMMAND_ARGS
#undef LH_RENDER_COMMAND_TYPES
