#include "pch.h"
#include "Mesh.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include <glad/glad.h>
namespace Luhame {
	namespace {
		const unsigned int import_flags =
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_PreTransformVertices |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_Debone |
			aiProcess_ValidateDataStructure;
	}

	struct log_stream : public Assimp::LogStream
	{
		static void initialize()
		{
			if (Assimp::DefaultLogger::isNullLogger())
			{
				Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
				Assimp::DefaultLogger::get()->attachStream(new log_stream, Assimp::Logger::Err | Assimp::Logger::Warn);
			}
		}

		void write(const char* message) override
		{
			LH_CORE_ERROR("Assimp error: {0}", message);
		}
	};

	mesh::mesh(const std::string& filename)
		: m_file_path(filename)
	{
		log_stream::initialize();//?

		LH_CORE_INFO("Loading mesh: {0}", filename.c_str());

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filename, import_flags);
		if (!scene || !scene->HasMeshes())
			LH_CORE_ERROR("Failed to load mesh file: {0}", filename);
		//暂时：这里只处理第一个mesh
		aiMesh* mesh = scene->mMeshes[0];

		LH_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
		LH_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

		m_vertices.reserve(mesh->mNumVertices);

		// Extract vertices from model
		for (size_t i = 0; i < m_vertices.capacity(); i++)
		{
			vertex vertex;
			vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
				vertex.binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}

			if (mesh->HasTextureCoords(0))
				vertex.texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			m_vertices.push_back(vertex);
		}

		m_vertex_buffer.reset(vertex_buffer::create());
		m_vertex_buffer->set_data(m_vertices.data(), m_vertices.size() * sizeof(vertex));

		// Extract indices from model
		m_indices.reserve(mesh->mNumFaces);
		for (size_t i = 0; i < m_indices.capacity(); i++)
		{
			LH_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
			m_indices.push_back({ mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] });
		}

		m_index_buffer.reset(index_buffer::create());
		m_index_buffer->set_data(m_indices.data(), m_indices.size() * sizeof(index));
	}

	mesh::~mesh()
	{
	}

	void mesh::render()
	{
		// TODO: Sort this out
		m_vertex_buffer->bind();
		m_index_buffer->bind();

		LH_RENDER_COMMAND_SUBMIT(
			LH_RENDER_COMMAND_TYPES(),
			LH_RENDER_COMMAND_ARGS(),
			(
				[]() {
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void*)offsetof(vertex, position));

					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void*)offsetof(vertex, normal));

					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void*)offsetof(vertex, tangent));

					glEnableVertexAttribArray(3);
					glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void*)offsetof(vertex, binormal));

					glEnableVertexAttribArray(4);
					glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void*)offsetof(vertex, texcoord));
				}
			)
		)
		draw_config config;
		renderer::draw_indexed(m_index_buffer->get_size(), config);
	}
}
