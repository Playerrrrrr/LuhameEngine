#pragma once

#include "Luhame/Core/Core.h"
#include "Luhame/Renderer/Renderer.h"

#include <string>

namespace Luhame
{
	struct LUHAME_API ShaderUniform
	{
		
	};

	struct LUHAME_API ShaderUniformCollection
	{

	};

	class LUHAME_API shader
	{
	public:
		virtual void bind() = 0;
		// Represents a complete shader program stored in a single file.
		// Note: currently for simplicity this is simply a string filepath, however
		//       in the future this will be an asset object + metadata
		static shader* create(const std::string& filepath);
	};

}