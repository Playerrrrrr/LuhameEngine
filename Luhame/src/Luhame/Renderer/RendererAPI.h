#pragma once
#include"Luhame/Core/Core.h"
namespace Luhame {
	using renderer_id = unsigned int;
	extern unsigned int vao;

	enum class LUHAME_API renderer_api_type
	{
		None,
		OpenGL,
		Vulkan
	};

	class renderer_api
	{
	private:
		static renderer_api_type s_current_renderer_api;
	public:


		static void init();
		static void Shutdown();

		static void draw_indexed(unsigned int count);

		static renderer_api_type current() { return s_current_renderer_api; }

		static void clear(float r, float g, float b, float a);
		static void set_clear_color(float r, float g, float b, float a);
		static void clear();
	};

}