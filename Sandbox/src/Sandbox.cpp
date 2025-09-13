#include "Luhame.h"
#include"Luhame/ImGui/ImGuiLayer.h"
#include<glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
//temp
namespace Game {

	static void ImGuiShowHelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
	class GameLayer:public Luhame::layer {
	public:
		GameLayer() = default;
		GameLayer(std::string layer_name)
			:layer(layer_name), 
			m_clear_color{ 0.2f, 0.3f, 0.8f, 1.0f },
			m_camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f)) {

		}
		// 通过 layer 继承
	private:
		void on_attach() override
		{
			m_simple_pbr_shader.reset(Luhame::shader::create("assets/shaders/simplepbr.glsl"));
			m_quad_shader.reset(Luhame::shader::create("assets/shaders/quad.glsl"));
			m_hdr_shader.reset(Luhame::shader::create("assets/shaders/hdr.glsl"));
			m_mesh.reset(new Luhame::mesh("assets/meshes/cerberus.fbx"));
			m_sphere_mesh.reset(new Luhame::mesh("assets/models/Sphere.fbx"));

			m_checkerboard_tex.reset(Luhame::texture_2d::create("assets/editor/Checkerboard.tga", false));

			//// Environment
			m_environment_cube_map.reset(Luhame::texture_cube::create("assets/textures/environments/Arches_E_PineTree_Radiance.tga"));
			m_environment_irradiance.reset(Luhame::texture_cube::create("assets/textures/environments/Arches_E_PineTree_Irradiance.tga"));
			m_brdf_lut.reset(Luhame::texture_2d::create("assets/textures/BRDF_LUT.tga",false));

			m_framebuffer.reset(Luhame::framebuffer::create(1280, 720, Luhame::framebuffer_format::RGBA16F));
			m_final_present_buffer.reset(Luhame::framebuffer::create(1280, 720, Luhame::framebuffer_format::RGBA8));

			// Create Quad
			float x = -1, y = -1;
			float width = 2, height = 2;
			struct quad_vertex
			{
				glm::vec3 position;
				glm::vec2 tex_coord;
			};
			quad_vertex* data = new quad_vertex[4];

			data[0].position = glm::vec3(x, y, 0);
			data[0].tex_coord = glm::vec2(0, 0);

			data[1].position = glm::vec3(x + width, y, 0);
			data[1].tex_coord = glm::vec2(1, 0);

			data[2].position = glm::vec3(x + width, y + height, 0);
			data[2].tex_coord = glm::vec2(1, 1);

			data[3].position = glm::vec3(x, y + height, 0);
			data[3].tex_coord = glm::vec2(0, 1);

			m_vertex_buffer.reset(Luhame::vertex_buffer::create());
			m_vertex_buffer->set_data(data, 4 * sizeof(quad_vertex));

			uint32_t* indices = new uint32_t[6]{ 0, 1, 2, 2, 3, 0, };
			m_index_buffer.reset(Luhame::index_buffer::create());
			m_index_buffer->set_data(indices, 6 * sizeof(unsigned int));

			m_light.direction = { -0.5f, -0.5f, 1.0f };
			m_light.radiance = { 1.0f, 1.0f, 1.0f };


	//		static float vertices[] = {
	//-0.5f, -0.5f, 0.0f,
	// 0.5f, -0.5f, 0.0f,
	// 0.0f,  0.5f, 0.0f
	//		};

	//		static unsigned int indices[] = {
	//			0, 1, 2
	//		};

	//		m_vertex_buffer = std::unique_ptr<Luhame::vertex_buffer>(Luhame::vertex_buffer::create());
	//		m_vertex_buffer->set_data(vertices, sizeof(vertices));

	//		m_index_buffer = std::unique_ptr<Luhame::index_buffer>(Luhame::index_buffer::create());
	//		m_index_buffer->set_data(indices, sizeof(indices));

			//m_quad_shader  = std::unique_ptr < Luhame::shader>( Luhame::shader::create("assets/shaders/quad.glsl"));

		}
		//解绑后在frame buffer 0 中绘制
		void on_detach() override
		{
		}
		void on_update(const Luhame::time_step& ts) override
		{
			m_camera.update(ts);
			auto view_projection = m_camera.get_projection_matrix()*m_camera.get_view_matrix();

			m_framebuffer->bind();
			Luhame::renderer::clear(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
			Luhame::uniform_buffer_declaration<sizeof(glm::mat4) + sizeof(int), 2> quad_shader_ub;
			quad_shader_ub.push("u_InverseVP", view_projection);
			quad_shader_ub.push("u_Texture", 0);
			m_quad_shader->bind();
			m_quad_shader->upload_uniform_buffer(quad_shader_ub);

			m_environment_irradiance->bind(0);
			m_vertex_buffer->bind();
			m_index_buffer->bind();
			Luhame::draw_config config;
			config.depth_test = false;
			Luhame::renderer::draw_indexed(m_index_buffer->get_count(), config);


			Luhame::uniform_buffer_declaration<sizeof(glm::mat4) * 2 + sizeof(glm::vec3) * 4 + sizeof(float) * 8 + sizeof(int)*3, 17> simple_pbr_shader_ub;
			simple_pbr_shader_ub.push("u_ViewProjectionMatrix", view_projection);
			simple_pbr_shader_ub.push("u_ModelMatrix", glm::mat4(1.0f));
			simple_pbr_shader_ub.push("u_AlbedoColor", m_albedo_input.color);
			simple_pbr_shader_ub.push("u_Metalness", m_metalness_input.value);
			simple_pbr_shader_ub.push("u_Roughness", m_roughness_input.value);
			simple_pbr_shader_ub.push("lights.Direction", m_light.direction);
			simple_pbr_shader_ub.push("lights.Radiance", m_light.radiance * m_light_multiplier);
			simple_pbr_shader_ub.push("u_CameraPosition", m_camera.get_position());
			simple_pbr_shader_ub.push("u_RadiancePrefilter", m_radiance_prefilter ? 1.0f : 0.0f);
			simple_pbr_shader_ub.push("u_AlbedoTexToggle", m_albedo_input.use_texture ? 1.0f : 0.0f);
			simple_pbr_shader_ub.push("u_NormalTexToggle", m_normal_input.use_texture ? 1.0f : 0.0f);
			simple_pbr_shader_ub.push("u_MetalnessTexToggle", m_metalness_input.use_texture ? 1.0f : 0.0f);
			simple_pbr_shader_ub.push("u_RoughnessTexToggle", m_roughness_input.use_texture ? 1.0f : 0.0f);
			simple_pbr_shader_ub.push("u_EnvMapRotation", m_env_map_rotation);
			m_simple_pbr_shader->upload_uniform_buffer(simple_pbr_shader_ub);
			m_environment_cube_map->bind(10);
			m_environment_irradiance->bind(11);
			m_brdf_lut->bind(15);

			m_simple_pbr_shader->bind();
			if (m_albedo_input.texture_map)
				m_albedo_input.texture_map->bind(1);
			if (m_normal_input.texture_map)
				m_normal_input.texture_map->bind(2);
			if (m_metalness_input.texture_map)
				m_metalness_input.texture_map->bind(3);
			if (m_roughness_input.texture_map)
				m_roughness_input.texture_map->bind(4);

			if (m_scene == scene::spheres)
			{
				// Metals
				float roughness = 0.0f;
				float x = -88.0f;
				for (int i = 0; i < 8; i++)
				{
					m_simple_pbr_shader->set_mat4("u_ModelMatrix", glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, 0.0f)));
					m_simple_pbr_shader->set_float("u_Roughness", roughness);
					m_simple_pbr_shader->set_float("u_Metalness", 1.0f);
					m_sphere_mesh->render();

					roughness += 0.15f;
					x += 22.0f;
				}

				// Dielectrics
				roughness = 0.0f;
				x = -88.0f;
				for (int i = 0; i < 8; i++)
				{
					m_simple_pbr_shader->set_mat4("u_ModelMatrix", glm::translate(glm::mat4(1.0f), glm:: vec3(x, 22.0f, 0.0f)));
					m_simple_pbr_shader->set_float("u_Roughness", roughness);
					m_simple_pbr_shader->set_float("u_Metalness", 0.0f);
					m_sphere_mesh->render();

					roughness += 0.15f;
					x += 22.0f;
				}

			}
			else if (m_scene == scene::model)
			{
				m_mesh->render();
			}
			m_framebuffer->unbind();
		
			m_final_present_buffer->bind();
			Luhame::renderer::clear(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
			m_hdr_shader->bind();
			m_hdr_shader->set_float("u_Exposure", m_exposure);
			m_framebuffer->bind_texture(0);
			m_hdr_shader->set_int("u_Texture", 0);
			m_vertex_buffer->bind();
			m_index_buffer->bind();
			Luhame::renderer::draw_indexed(m_index_buffer->get_count(), config);
			m_final_present_buffer->unbind();
		}
		void on_event(Luhame::event& event) override
		{
		}
		void on_ui_render() override{

			static bool show_demo_window = true;
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			ImGui::Begin("GameLayer");
			ImGui::ColorEdit4("Clear Color", m_clear_color);
			ImGui::DragFloat("roughness", &m_roughness_input.value, 0.01);
			ImGui::DragFloat("metallic", &m_metalness_input.value, 0.01);
			ImGui::ColorEdit3("color", &m_albedo_input.color[0]);
			ImGui::End();

			ImGui::Begin("settings");

			if (ImGui::TreeNode("shaders")) {
				auto& shaders = Luhame::shader::s_all_shaders;
				for (auto& t : shaders) {
					if (ImGui::TreeNode(t.second->get_name().c_str())) {
						std::string button_name = "Reload##" + t.second->get_name();
						if (ImGui::Button(button_name.c_str())) {
							t.second->reload();
						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
			}

			ImGui::RadioButton("sphere", (int*)&m_scene, (int)scene::spheres);
			ImGui::SameLine();
			ImGui::RadioButton("model", (int*)&m_scene, (int)scene::model);
			ImGui::SliderFloat3("light dir", &m_light.direction[0], -1, 1);
			ImGui::ColorEdit3("light radiance", &m_light.radiance[0]);
			ImGui::SliderFloat("light strength", &m_light_multiplier, 0,100);
			ImGui::SliderFloat("exposure", &m_exposure,0,10);
			auto cameraForward = m_camera.get_forward_direction();
			ImGui::Text("Camera Forward: %.2f, %.2f, %.2f", cameraForward.x, cameraForward.y, cameraForward.z);

			ImGui::Separator();

			ImGui::Text("mesh");
			std::string full_path = m_mesh?m_mesh->get_file_path():"None";
			auto find = full_path.find_last_of("/\\");
			std::string mesh_name = (find != std::string::npos) ? full_path.substr(find + 1) : full_path;
			ImGui::Text(mesh_name.c_str());
			ImGui::SameLine();
			if (ImGui::Button("...##mesh")) {
				std::string name = Luhame::Application::get()->open_file("");
				if (name != "") {
					m_mesh.reset(new Luhame::mesh{ name });
				}
			}
			ImGui::Separator();

			ImGui::Text("Shader Parameters");
			ImGui::Checkbox("Radiance Prefiltering", &m_radiance_prefilter);
			ImGui::SliderFloat("Env Map Rotation", &m_env_map_rotation, -360.0f, 360.0f);
			ImGui::Separator();

			//texture 
			{

				if (ImGui::CollapsingHeader("Albedo", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
					ImGui::Image(m_albedo_input.texture_map ? (void*)m_albedo_input.texture_map->get_renderer_id() : (void*)m_checkerboard_tex->get_renderer_id(), ImVec2(64, 64));
					ImGui::PopStyleVar();
					if (ImGui::IsItemHovered())
					{
						if (m_albedo_input.texture_map)
						{
							ImGui::BeginTooltip();
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
							ImGui::TextUnformatted(m_albedo_input.texture_map->get_path().c_str());
							ImGui::PopTextWrapPos();
							ImGui::Image((void*)m_albedo_input.texture_map->get_renderer_id(), ImVec2(384, 384));
							ImGui::EndTooltip();
						}
						if (ImGui::IsItemClicked())
						{
							std::string filename = Luhame::Application::get()->open_file("");
							if (filename != "")
								m_albedo_input.texture_map.reset(Luhame::texture_2d::create(filename, m_albedo_input.srgb));
						}
					}
					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::Checkbox("Use##AlbedoMap", &m_albedo_input.use_texture);
					if (ImGui::Checkbox("sRGB##AlbedoMap", &m_albedo_input.srgb))
					{
						if (m_albedo_input.texture_map)
							m_albedo_input.texture_map.reset(Luhame::texture_2d::create(m_albedo_input.texture_map->get_path(), m_albedo_input.srgb));
					}
					ImGui::EndGroup();
					ImGui::SameLine();
					ImGui::ColorEdit3("Color##Albedo", glm::value_ptr(m_albedo_input.color), ImGuiColorEditFlags_NoInputs);
				}

				if (ImGui::CollapsingHeader("Normals", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
					ImGui::Image(m_normal_input.texture_map ? (void*)m_normal_input.texture_map->get_renderer_id() : (void*)m_checkerboard_tex->get_renderer_id(), ImVec2(64, 64));
					ImGui::PopStyleVar();
					if (ImGui::IsItemHovered())
					{
						if (m_normal_input.texture_map)
						{
							ImGui::BeginTooltip();
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
							ImGui::TextUnformatted(m_normal_input.texture_map->get_path().c_str());
							ImGui::PopTextWrapPos();
							ImGui::Image((void*)m_normal_input.texture_map->get_renderer_id(), ImVec2(384, 384));
							ImGui::EndTooltip();
						}
						if (ImGui::IsItemClicked())
						{
							std::string filename = Luhame::Application::get()->open_file("");
							if (filename != "")
								m_normal_input.texture_map.reset(Luhame::texture_2d::create(filename,false));
						}
					}
					ImGui::SameLine();
					ImGui::Checkbox("Use##NormalMap", &m_normal_input.use_texture);
				}

				if (ImGui::CollapsingHeader("Metalness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
					ImGui::Image(m_metalness_input.texture_map ? (void*)m_metalness_input.texture_map->get_renderer_id() : (void*)m_checkerboard_tex->get_renderer_id(), ImVec2(64, 64));
					ImGui::PopStyleVar();
					if (ImGui::IsItemHovered())
					{
						if (m_metalness_input.texture_map)
						{
							ImGui::BeginTooltip();
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
							ImGui::TextUnformatted(m_metalness_input.texture_map->get_path().c_str());
							ImGui::PopTextWrapPos();
							ImGui::Image((void*)m_metalness_input.texture_map->get_renderer_id(), ImVec2(384, 384));
							ImGui::EndTooltip();
						}
						if (ImGui::IsItemClicked())
						{
							std::string filename = Luhame::Application::get()->open_file("");
							if (filename != "")
								m_metalness_input.texture_map.reset(Luhame::texture_2d::create(filename,false));
						}
					}
					ImGui::SameLine();
					ImGui::Checkbox("Use##MetalnessMap", &m_metalness_input.use_texture);
					ImGui::SameLine();
					ImGui::SliderFloat("Value##MetalnessInput", &m_metalness_input.value, 0.0f, 1.0f);
				}

				if (ImGui::CollapsingHeader("Roughness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
					ImGui::Image(m_roughness_input.texture_map ? (void*)m_roughness_input.texture_map->get_renderer_id() : (void*)m_checkerboard_tex->get_renderer_id(), ImVec2(64, 64));
					ImGui::PopStyleVar();
					if (ImGui::IsItemHovered())
					{
						if (m_roughness_input.texture_map)
						{
							ImGui::BeginTooltip();
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
							ImGui::TextUnformatted(m_roughness_input.texture_map->get_path().c_str());
							ImGui::PopTextWrapPos();
							ImGui::Image((void*)m_roughness_input.texture_map->get_renderer_id(), ImVec2(384, 384));
							ImGui::EndTooltip();
						}
						if (ImGui::IsItemClicked())
						{
							std::string filename = Luhame::Application::get()->open_file("");
							if (filename != "")
								m_roughness_input.texture_map.reset(Luhame::texture_2d::create(filename,false));
						}
					}
					ImGui::SameLine();
					ImGui::Checkbox("Use##RoughnessMap", &m_roughness_input.use_texture);
					ImGui::SameLine();
					ImGui::SliderFloat("Value##RoughnessInput", &m_roughness_input.value, 0.0f, 1.0f);
				}


				ImGui::Separator();

			}

			ImGui::End();

			ImGui::Begin("Render ViewPort");
			ImGui::Image(m_final_present_buffer->get_color_attachment_renderer_id(),
				ImVec2(Luhame::Application::get()->get_window().get_width(),
					Luhame::Application::get()->get_window().get_height()),
				{1,1},{0,0});
			ImGui::End();
		}



		float m_clear_color[4];

		std::unique_ptr<Luhame::vertex_buffer> m_VB;
		std::unique_ptr<Luhame::index_buffer> m_IB;
		std::unique_ptr<Luhame::shader> m_Shader;
		std::unique_ptr<Luhame::shader> m_shader;
		std::unique_ptr<Luhame::shader> m_pbr_shader;
		std::unique_ptr<Luhame::shader> m_simple_pbr_shader;
		std::unique_ptr<Luhame::shader> m_quad_shader;
		std::unique_ptr<Luhame::shader> m_hdr_shader;
		std::unique_ptr<Luhame::mesh> m_mesh;
		std::unique_ptr<Luhame::mesh> m_sphere_mesh;
		std::unique_ptr<Luhame::texture_2d> m_brdf_lut;


		struct albedo_input
		{
			glm::vec3 color = { 0.972f, 0.96f, 0.915f }; // Silver, from https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
			std::unique_ptr<Luhame::texture_2d> texture_map;
			bool srgb = true;
			bool use_texture = false;
		};
		albedo_input m_albedo_input;

		struct normal_input
		{
			std::unique_ptr<Luhame::texture_2d> texture_map;
			bool use_texture = false;
		};
		normal_input m_normal_input;

		struct metalness_input
		{
			float value = 1.0f;
			std::unique_ptr<Luhame::texture_2d> texture_map;
			bool use_texture = false;
		};
		metalness_input m_metalness_input;

		struct roughness_input
		{
			float value = 0.5f;
			std::unique_ptr<Luhame::texture_2d> texture_map;
			bool use_texture = false;
		};
		roughness_input m_roughness_input;

		std::unique_ptr<Luhame::framebuffer> m_framebuffer, m_final_present_buffer;

		std::unique_ptr<Luhame::vertex_buffer> m_vertex_buffer;
		std::unique_ptr<Luhame::index_buffer> m_index_buffer;
		std::unique_ptr<Luhame::texture_cube> m_environment_cube_map, m_environment_irradiance;

		Luhame::camera m_camera;


		struct light
		{
			glm::vec3 direction;
			glm::vec3 radiance;
		};
		light m_light;
		float m_light_multiplier = 0.3f;

		// PBR params
		float m_exposure = 1.0f;

		bool m_radiance_prefilter = false;

		float m_env_map_rotation = 0.0f;

		enum class scene : uint32_t
		{
			spheres = 0, model = 1
		};
		scene m_scene = scene::model;

		// Editor resources
		std::unique_ptr<Luhame::texture_2d> m_checkerboard_tex;

		unsigned int vao;
	};

	class Sandbox : public Luhame::Application
	{
	public:
		Sandbox()
			:Application() {
			LH_TRACE("Hello!");
			push_layer(new GameLayer{"Game layer"});
			//push_layer(new Luhame::imgui_layer());
		}

	};
}
Luhame::Application* Luhame::CreateApplication()
{
	return new Game::Sandbox();
}
//Playerrrrrr
//Kk876942392