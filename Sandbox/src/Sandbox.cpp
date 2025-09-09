#include "Luhame.h"
#include"Luhame/ImGui/ImGuiLayer.h"

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
			:layer(layer_name), m_clear_color{ 0.2f, 0.3f, 0.8f, 1.0f } { }
		// 通过 layer 继承
	private:
		void on_attach() override
		{
			static float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
			};

			static unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
			};

			m_VB = std::unique_ptr<Luhame::vertex_buffer>(Luhame::vertex_buffer::create());
			m_VB->set_data(vertices, sizeof(vertices));

			m_IB = std::unique_ptr<Luhame::index_buffer>(Luhame::index_buffer::create());
			m_IB->set_data(indices, sizeof(indices));
			m_Shader.reset(Luhame::shader::create("asset/shader/shader.glsl"));
		}
		void on_detach() override
		{
		}
		void on_update() override
		{
			Luhame::renderer::clear(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
			m_VB->bind();
			m_IB->bind();
			m_Shader->bind();//这个一定要绑定，没次会刷新
			Luhame::renderer::draw_indexed(6);
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
			ImGui::End();
		}
		float m_clear_color[4];

		std::unique_ptr<Luhame::vertex_buffer> m_VB;
		std::unique_ptr<Luhame::index_buffer> m_IB;
		std::unique_ptr<Luhame::shader> m_Shader;


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