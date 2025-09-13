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

	struct renderer_api_capabilities {
		std::string vendor;
		std::string renderer;
		std::string version;

		int max_samples;
		float max_anisotropy = 1;
	};

	struct draw_config {
		bool depth_test = true;
	};

	class renderer_api
	{
	private:
		static renderer_api_type s_current_renderer_api;
	public:


		static void init();
		static void Shutdown();

		static void draw_indexed(unsigned int count, draw_config config);

		static renderer_api_type current() { return s_current_renderer_api; }

		static renderer_api_capabilities& get_capabilities() {
			static renderer_api_capabilities capabilities;
			return capabilities;
		}
		static void clear(float r, float g, float b, float a);
		static void set_clear_color(float r, float g, float b, float a);
		static void clear();
	};

}