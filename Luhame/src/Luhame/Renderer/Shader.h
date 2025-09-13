#pragma once

#include "Luhame/Core/Core.h"
#include "Luhame/Renderer/Renderer.h"

#include <string>
#include"glm/glm.hpp"

namespace Luhame
{
	struct LUHAME_API shader_uniform
	{

	};

	struct LUHAME_API shader_uniform_collection
	{

	};

	enum class LUHAME_API uniform_type
	{
		none = 0,
		float1, float2, float3, float4,
		matrix3x3, matrix4x4,
		int32, uint32
	};

	struct LUHAME_API uniform_decl
	{
		uniform_type type;
		std::ptrdiff_t offset;
		std::string name;
	};

	struct LUHAME_API uniform_buffer
	{
		// TODO: This currently represents a byte buffer that has been
		// packed with uniforms. This was primarily created for OpenGL,
		// and needs to be revisted for other rendering APIs. Furthermore,
		// this currently does not assume any alignment. This also has
		// nothing to do with GL uniform buffers, this is simply a CPU-side
		// buffer abstraction.
		byte* buffer;
		std::vector<uniform_decl> uniforms;
	};

	struct uniform_buffer_base
	{
		virtual const byte* get_buffer() const = 0;
		virtual const uniform_decl* get_uniforms() const = 0;
		virtual unsigned int get_uniform_count() const = 0;
	};

	template<unsigned int N, unsigned int U>
	struct uniform_buffer_declaration : public uniform_buffer_base
	{
		byte buffer[N];
		uniform_decl uniforms[U];
		std::ptrdiff_t cursor = 0;
		uint32_t uniform_count = 0;


		virtual const byte* get_buffer() const override { return buffer; }
		virtual const uniform_decl* get_uniforms() const override { return uniforms; }
		virtual unsigned int get_uniform_count() const { return U; }

		template<typename T>
		void push(const std::string& name, const T& data) {
			LH_CORE_ERROR("{} not support for now", typeid(T).name());
		}

		template<>
		void push(const std::string& name, const float& data)
		{
			uniforms[uniform_count++] = { uniform_type::float1, cursor, name };
			memcpy(buffer + cursor, &data, sizeof(float));
			cursor += sizeof(float);
		}

		template<>
		void push(const std::string& name, const glm::vec3& data)
		{
			uniforms[uniform_count++] = { uniform_type::float3, cursor, name };
			memcpy(buffer + cursor, glm::value_ptr(data), sizeof(glm::vec3));
			cursor += sizeof(glm::vec3);
		}

		template<>
		void push(const std::string& name, const int& data)
		{
			uniforms[uniform_count++] = { uniform_type::int32, cursor, name };
			memcpy(buffer + cursor,&data, sizeof(int));
			cursor += sizeof(int);
		}

		template<>
		void push(const std::string& name, const glm::vec4& data)
		{
			uniforms[uniform_count++] = { uniform_type::float4, cursor, name };
			memcpy(buffer + cursor, glm::value_ptr(data), sizeof(glm::vec4));
			cursor += sizeof(glm::vec4);
		}
		template<>
		void push(const std::string& name, const glm::mat4& data)
		{
			uniforms[uniform_count++] = { uniform_type::matrix4x4, cursor, name };
			memcpy(buffer + cursor, glm::value_ptr(data), sizeof(glm::mat4));
			cursor += sizeof(glm::mat4);
		}

	};
	class LUHAME_API shader
	{
	public:
		virtual void bind() = 0;

		virtual void reload() = 0;

		virtual void set_float(const std::string& name, float value) = 0;
		virtual void set_mat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void set_int(const std::string& name, int value) = 0;

		virtual const std::string& get_name() const  = 0;
		// Represents a complete shader program stored in a single file.
		// Note: currently for simplicity this is simply a string filepath, however
		//       in the future this will be an asset object + metadata
		virtual void upload_uniform_buffer(const uniform_buffer_base& uniform_buffer) = 0;
		static shader* create(const std::string& filepath);
		//temporary
		inline static std::unordered_map<std::string,shader*> s_all_shaders;
	};

}