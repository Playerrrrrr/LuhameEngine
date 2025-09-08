#include "Luhame.h"
#include"Luhame/ImGui/ImGuiLayer.h"

namespace Game {
	
	class GameLayer:public Luhame::layer {
	public:
		GameLayer() = default;
		GameLayer(std::string layer_name)
			:layer(layer_name){ }
		// Í¨¹ý layer ¼Ì³Ð
	private:
		void on_attach() override
		{
		}
		void on_detach() override
		{
		}
		void on_update() override
		{
		}
		void on_event(Luhame::event& event) override
		{
		}
	};

	class Sandbox : public Luhame::Application
	{
	public:
		Sandbox()
			:Application() {
			LH_TRACE("Hello!");
			push_layer(new GameLayer{"Game layer"});
			push_layer(new Luhame::imgui_layer());
		}

	};
}
Luhame::Application* Luhame::CreateApplication()
{
	return new Game::Sandbox();
}
//Playerrrrrr
//Kk876942392