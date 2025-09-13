#pragma once
#include"Luhame/Renderer/Buffer.h"
#include"glm/glm.hpp"
namespace Luhame {

	class mesh
	{
	public:
		struct vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec3 tangent;
			glm::vec3 binormal;//¸±ÇÐÏß£¿
			glm::vec2 texcoord;
		};
		static_assert(sizeof(vertex) == 14 * sizeof(float));
		inline static const int num_attributes = 5;

		struct index
		{
			uint32_t v1, v2, v3;
		};
		static_assert(sizeof(index) == 3 * sizeof(uint32_t));

		mesh(const std::string& filename);
		~mesh();

		void render();

		inline const std::string& get_file_path() const { return m_file_path; }
	private:
		std::vector<vertex> m_vertices;
		std::vector<index> m_indices;

		std::unique_ptr<vertex_buffer> m_vertex_buffer;
		std::unique_ptr<index_buffer> m_index_buffer;

		std::string m_file_path;
	};
}

