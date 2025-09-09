#pragma once
#include"Luhame/Core/Core.h"
namespace Luhame {
	class LUHAME_API render_command_queue
	{
	public:
		//约定render_command函数返回参数的大小
		//typedef unsigned int(*render_command_fn)(void*);
		using render_command_fn = void (*)(void*);
		using command_para_size = unsigned int;
		render_command_queue();
		~render_command_queue();

		void* allocate(render_command_fn func, command_para_size size);

		void execute();
	private:
		//内存布局 element : [fun(render_command_fn)][size(unsigned int)][byte of size]
		// m_command_buffer [element][element][element]...
		unsigned char* m_command_buffer;
		unsigned char* m_command_buffer_ptr;
		unsigned int m_render_command_count = 0;
	};
}

