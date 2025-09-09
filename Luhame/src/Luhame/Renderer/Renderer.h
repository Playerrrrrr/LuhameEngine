#pragma once

#include "RenderCommandQueue.h"
#include "RendererAPI.h"

/*
			A ==== B   A����B
			A     [B]  A����B
			A ---- B   A����B
			renderer ==== render_comand_queue
			render_comand_queue [render_command]
			render_command ---- renderer_api

	user �ύrenderer command  --> renderer ��renderer command push to render_comand_queue
	user ִ��renderer command  --> renderer ִ�� render_comand_queue --> ִ�е��õ�renderer command -->renderer commandִ�� renderer API������ʵ���ǲ�ͬ��ͼ��API��

*/

template<class...Args>
struct LH_RENDERER_COMMAND {
	static void excute(void* self) {
		LH_RENDERER_COMMAND& command = *static_cast<LH_RENDERER_COMMAND*>(self);
		std::apply(command.m_fun, command.m_args);
	}
	std::function<void(Args...)> m_fun;
	std::tuple<Args...> m_args;

	LH_RENDERER_COMMAND(std::function<void(Args...)> fun, Args... args)
		: m_fun(fun), m_args(std::forward<Args>(args)...) {
	}
};
template<>
struct LH_RENDERER_COMMAND<> {
	static void excute(void* self) {
		LH_RENDERER_COMMAND& command = *static_cast<LH_RENDERER_COMMAND*>(self);
		command.m_fun();
	}
	std::function<void(void)> m_fun;

	LH_RENDERER_COMMAND(std::function<void(void)> fun )
		: m_fun(fun) {
	}
};

#define LH_RENDER_COMMAND_TYPES(...)\
	LH_RENDERER_COMMAND<__VA_ARGS__>
#define LH_RENDER_COMMAND_ARGS(...)\
	__VA_ARGS__
#define LH_RENDER_COMMAND_FUNCTION_BEGIN (
#define LH_RENDER_COMMAND_FUNCTION_END )

#define LH_RENDER_COMMAND_SUBMIT(CommandType,CommandArgs, LambdaBody) \
    { \
        void* mem = ::Luhame::renderer::submit( \
            CommandType::excute, \
            sizeof(CommandType) \
        ); \
        new(mem) CommandType{ LambdaBody, CommandArgs }; \
    }
//�ڴ������place new

namespace Luhame {
	class LUHAME_API renderer
	{
	private:

	public:
		// Commands
		static void clear();
		static void clear(float r, float g, float b, float a = 1.0f);
		static void set_clear_color(float r, float g, float b, float a);

		static void clear_magenta();

		static void init();

		static void draw_indexed(unsigned int count);


		static void* submit(render_command_queue::render_command_fn command,unsigned int size)
		{
			return s_instance->m_command_queue.allocate(command, size);
		}

		void wait_and_render();

		inline static renderer& get() { return *s_instance; }
	private:
		static renderer* s_instance;

		render_command_queue m_command_queue;
	};

//#define HZ_RENDER(x) ::Luhame::renderer::submit([=](){ renderer_api::x; })
}	

