#pragma once

#include "Luhame/Core/Core.h"
#include"Luhame/Renderer/RendererAPI.h"
namespace Luhame {

	enum class LUHAME_API texture_format
	{
		None = 0,
		GREY,
		RGB ,
		RGBA ,
	};

	class LUHAME_API texture
	{
	public:
		virtual ~texture() {}
		virtual renderer_id get_renderer_id() const = 0;
	};

	class LUHAME_API texture_2d : public texture
	{
	public:
		static texture_2d* create(texture_format format, unsigned int width, unsigned int height);
		static texture_2d* create(const std::string& file_path, bool srgb);
		virtual void bind(uint32_t slot) const = 0;
		virtual texture_format get_format() const = 0;
		virtual unsigned int get_width() const = 0;
		virtual unsigned int get_height() const = 0;
		virtual const std::string& get_path() const = 0;
		virtual void release() = 0;
	};

	class texture_cube : public texture {
	public:
		static texture_cube* create(const std::string& file_path);
		virtual void bind(uint32_t slot = 0) const = 0;
		virtual texture_format get_format() const = 0;
		virtual unsigned int get_width() const = 0;
		virtual unsigned int get_height() const = 0;
		virtual const std::string& get_path() const = 0;
		virtual void release() = 0;
	};

}