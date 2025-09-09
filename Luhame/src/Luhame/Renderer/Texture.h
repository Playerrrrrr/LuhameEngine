#pragma once

#include "Luhame/Core/Core.h"

namespace Luhame {

	enum class LUHAME_API texture_format
	{
		None = 0,
		RGB = 1,
		RGBA = 2,
	};

	class LUHAME_API texture
	{
	public:
		virtual ~texture() {}
	};

	class LUHAME_API texture_2d : public texture
	{
	public:
		static texture_2d* create(texture_format format, unsigned int width, unsigned int height);

		virtual texture_format get_format() const = 0;
		virtual unsigned int get_width() const = 0;
		virtual unsigned int get_height() const = 0;
	};

}