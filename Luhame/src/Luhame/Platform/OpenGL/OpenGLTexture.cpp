#include "pch.h"
#include "OpenGLTexture.h"

#include "Luhame/Renderer/RendererAPI.h"
#include "Luhame/Renderer/Renderer.h"

#include <Glad/glad.h>
#include"stb/stb_image.h"

namespace Luhame {

	static GLenum luhame_to_opengl_texture_format(texture_format format)
	{
		switch (format)
		{
		case Luhame::texture_format::GREY:     return GL_RED;
		case Luhame::texture_format::RGB:     return GL_RGB;
		case Luhame::texture_format::RGBA:    return GL_RGBA;
		}
		return 0;
	}

	static int calculate_mipmap_count(int width, int height) {
		int levels = 1;
		while ((width | height) >> levels) {
			levels++;
		}
		return levels;
	}

	opengl_texture_2d::opengl_texture_2d(texture_format format, unsigned int width, unsigned int height)
		: m_format(format), m_width(width), m_height(height)
	{

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(
				opengl_texture_2d* ,
				texture_format,
				unsigned int,
				unsigned int),
			LH_RENDER_COMMAND_ARGS(
				this,
				m_format, 
				m_width, 
				m_height),
			([](opengl_texture_2d*self,texture_format l_texture_format,unsigned int l_width,unsigned int l_height) {
				glGenTextures(1, &self->m_renderer_id);
				glBindTexture(GL_TEXTURE_2D, self->m_renderer_id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				//没有初始化max_anisotropy
				glTextureParameterf(self->m_renderer_id, GL_TEXTURE_MAX_ANISOTROPY, renderer_api::get_capabilities().max_anisotropy);
				glTexImage2D(
					GL_TEXTURE_2D, 0, 
					luhame_to_opengl_texture_format(l_texture_format), 
					l_width, l_height, 
					0, 
					luhame_to_opengl_texture_format(l_texture_format),
					GL_UNSIGNED_BYTE, nullptr
				);
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
				self->has_state = true;
			})
		)
	}

	opengl_texture_2d::~opengl_texture_2d()
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(renderer_id*),
			LH_RENDER_COMMAND_ARGS(&m_renderer_id),
			(
				[](renderer_id* l_renderer_id) {
					glDeleteTextures(1, l_renderer_id);
				}
			)
		)
	}

	opengl_texture_2d::opengl_texture_2d(const std::string& file_path, bool srgb)
	:m_file_path(file_path){
		int width, height, channals;
		m_image_data = stbi_load(file_path.c_str(), &width, &height, &channals,0);
		m_width = width;
		m_height = height;
		//m_format = texture_format::RGBA;
		//这句会出现环形错误显示
		switch (channals) {
		case 1: { m_format = texture_format::GREY; break; };
		case 3: { m_format = texture_format::RGB; break;};
		case 4: { m_format = texture_format::RGBA; break;};
		}
		LH_CORE_INFO("h w c {0} {1} {2}", m_width, m_height, (int)m_format);

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(opengl_texture_2d*,bool ),
			LH_RENDER_COMMAND_ARGS(this, srgb),
			(
				[](opengl_texture_2d* self,bool srgb) {

					LH_CORE_INFO("Loading texture : {}", self->m_file_path);
					if (srgb)
					{
						glCreateTextures(GL_TEXTURE_2D, 1, &self->m_renderer_id);
						int levels = calculate_mipmap_count(self->m_width, self->m_height);
						LH_CORE_INFO("levels {}", levels);
						LH_CORE_INFO("Creating srgb texture width {0} mips", levels);
						//glTextureStorage2D相对glTexParameteri，作用一样，但是API风格不一样，前者是直接状态访问，后者是状态机，也就是前者需要一个新参数：id
						glTextureStorage2D(self->m_renderer_id, levels, GL_SRGB8, self->m_width, self->m_height);
						glTextureParameteri(self->m_renderer_id, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
						glTextureParameteri(self->m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

						glTextureSubImage2D(self->m_renderer_id, 0, 0, 0, self->m_width, self->m_height, GL_RGB, GL_UNSIGNED_BYTE, self->m_image_data);
						glGenerateTextureMipmap(self->m_renderer_id);
					}
					else
					{
						LH_CORE_INFO("here");
						glGenTextures(1, &self->m_renderer_id);
						glBindTexture(GL_TEXTURE_2D, self->m_renderer_id);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
						LH_CORE_INFO("here 1");

						glTexImage2D(
							GL_TEXTURE_2D, 
							0, 
							luhame_to_opengl_texture_format(self->m_format), 
							self->m_width, self->m_height, 
							0, 
							luhame_to_opengl_texture_format(self->m_format), 
							GL_UNSIGNED_BYTE, 
							self->m_image_data
						);
						LH_CORE_INFO("here 2");

						glGenerateMipmap(GL_TEXTURE_2D);

						glBindTexture(GL_TEXTURE_2D, 0);
					}
					LH_CORE_INFO("here");
					stbi_image_free(self->m_image_data);
					self->has_state = true;
				}
			)
		)
	}

	void opengl_texture_2d::bind(uint32_t slot) const
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(decltype(this), decltype(slot)),
			LH_RENDER_COMMAND_ARGS(this, slot),
			(
				[](decltype(this) self,uint32_t slot) {
					if (!self->has_state)
						LH_CORE_ERROR("trying to bind a texture which not have state for opengl");
					glBindTextureUnit(slot, self->m_renderer_id);
				}
			)
		)
	}

	void opengl_texture_2d::release() {
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(opengl_texture_2d*),
			LH_RENDER_COMMAND_ARGS(this),
			(

				[](opengl_texture_2d* self) {
					if (!self->has_state)
						LH_CORE_ERROR("trying to release a texture which not have state for opengl");
					glDeleteTextures(1, &self->m_renderer_id);
					self->has_state = false;
				}
			)
		)
	}




	opengl_texture_cube::opengl_texture_cube(const std::string& file_path)
	:m_file_path(file_path){
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		m_image_data = stbi_load(file_path.c_str(), &width, &height, &channels, STBI_rgb);
		m_width = width;
		m_height = height;
		m_format = channels == 3 ? texture_format::RGB : texture_format::RGBA;
		/*
		*		o
		*     o o o
		*       o
		*       o
		*/
		unsigned int face_width = m_width / 4;
		unsigned int face_height = m_height / 3;
		LH_CORE_ASSERT(face_width == face_height, "Non-square faces!");
		std::array<unsigned char*, 6> faces;
		for (size_t i = 0; i < faces.size(); i++)
			faces[i] = new unsigned char[face_width * face_height * 3]; // 3 BPP
		int faceIndex = 0;
		/*
			读取 o
				 o
				 o
				 o
		*/
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t y = 0; y < face_height; y++)
			{
				size_t yOffset = y + face_height;
				for (size_t x = 0; x < face_width; x++)
				{
					size_t xOffset = x + i * face_width;
					faces[faceIndex][(x + y * face_width) * 3 + 0] = m_image_data[(xOffset + yOffset * m_width) * 3 + 0];
					faces[faceIndex][(x + y * face_width) * 3 + 1] = m_image_data[(xOffset + yOffset * m_width) * 3 + 1];
					faces[faceIndex][(x + y * face_width) * 3 + 2] = m_image_data[(xOffset + yOffset * m_width) * 3 + 2];
				}
			}
			faceIndex++;
		}
		/*
			读取 o   o
		*/
		for (size_t i = 0; i < 3; i++)
		{
			// Skip the middle one
			if (i == 1)
				continue;

			for (size_t y = 0; y < face_height; y++)
			{
				size_t yOffset = y + i * face_height;
				for (size_t x = 0; x < face_width; x++)
				{
					size_t xOffset = x + face_width;
					faces[faceIndex][(x + y * face_width) * 3 + 0] = m_image_data[(xOffset + yOffset * m_width) * 3 + 0];
					faces[faceIndex][(x + y * face_width) * 3 + 1] = m_image_data[(xOffset + yOffset * m_width) * 3 + 1];
					faces[faceIndex][(x + y * face_width) * 3 + 2] = m_image_data[(xOffset + yOffset * m_width) * 3 + 2];
				}
			}
			faceIndex++;
		}

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(opengl_texture_cube*, std::array<unsigned char*, 6>, uint32_t, uint32_t),
			LH_RENDER_COMMAND_ARGS(this, faces, face_width, face_height),
			(
				[](opengl_texture_cube* self, std::array<unsigned char*, 6> faces, uint32_t face_width, uint32_t face_height) {
					LH_CORE_INFO("create cube {}", self->m_file_path);
					glGenTextures(1, &self->m_renderer_id);
					glBindTexture(GL_TEXTURE_CUBE_MAP, self->m_renderer_id);

					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

					glTextureParameterf(self->m_renderer_id, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);

					auto format = luhame_to_opengl_texture_format(self->m_format);

					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, face_width, face_height, 0, format, GL_UNSIGNED_BYTE, faces[2]);
					glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, face_width, face_height, 0, format, GL_UNSIGNED_BYTE, faces[0]);

					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, face_width, face_height, 0, format, GL_UNSIGNED_BYTE, faces[4]);
					glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, face_width, face_height, 0, format, GL_UNSIGNED_BYTE, faces[5]);

					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, face_width, face_height, 0, format, GL_UNSIGNED_BYTE, faces[1]);
					glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, face_width, face_height, 0, format, GL_UNSIGNED_BYTE, faces[3]);

					glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

					glBindTexture(GL_TEXTURE_2D, 0);
					for (size_t i = 0; i < faces.size(); i++)
						delete[] faces[i];

					stbi_image_free(self->m_image_data);
					self->has_state = true;
				}
			)
		)
	}

	opengl_texture_cube::~opengl_texture_cube()
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(uint32_t),
			LH_RENDER_COMMAND_ARGS(this->m_renderer_id),
			[](uint32_t rd_id) {
				glDeleteTextures(1, &rd_id);
			}
		)
	}

	void opengl_texture_cube::bind(uint32_t slot) const
	{
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(const opengl_texture_cube*, uint32_t),
			LH_RENDER_COMMAND_ARGS(this, slot),
			(
				[](const opengl_texture_cube* self, uint32_t slot) {
					if (!self->has_state)
						LH_CORE_ERROR("trying to bind a texture which not have state for opengl");
					if constexpr (LH_RENDERER_DEBUG)

					LH_CORE_INFO("use cube map : {}", self->m_renderer_id);

					glActiveTexture(GL_TEXTURE0 + slot);
					glBindTexture(GL_TEXTURE_CUBE_MAP, self->m_renderer_id);
				}
			)
		)
	}

	void opengl_texture_cube::release() {
		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(opengl_texture_cube*),
			LH_RENDER_COMMAND_ARGS(this),
			(
				[](opengl_texture_cube* self) {
					if (!self->has_state)
						LH_CORE_ERROR("trying to release a texture which not have state for opengl");
					glDeleteTextures(1, &self->m_renderer_id);
				}
			)
		)
	}

}