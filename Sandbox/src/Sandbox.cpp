#include "Luhame.h"

class Sandbox : public Luhame::Application
{
public:
	Sandbox()
	{
		LH_TRACE("Hello!");
	}
};

Luhame::Application* Luhame::CreateApplication()
{
	return new Sandbox();
}
//Playerrrrrr
//Kk876942392