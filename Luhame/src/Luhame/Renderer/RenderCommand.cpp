#include "pch.h"
#include "RenderCommand.h"
#include"RendererAPI.h"
namespace Luhame {
	unsigned int render_command::clear(void* datablock)
	{
		float* data = (float*)datablock;

		float r = *data++;
		float g = *data++;
		float b = *data++;
		float a = *data;

		renderer_api::clear(r, g, b, a);

		return sizeof(float) * 4;
	}

}
