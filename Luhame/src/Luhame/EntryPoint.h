#pragma once

#ifdef LH_PLATFORM_WINDOWS
//.h�ļ���Ψһ����ʱ�������ԭ�ļ����ᱻ���룿
extern Luhame::Application* Luhame::CreateApplication();

int main(int argc, char** argv)
{
	Luhame::Application* app = Luhame::CreateApplication();
	LH_CORE_ASSERT(app, "Client application is null!");
	app->Run();
	delete app;
}
#else
	#error Luhame only supports Windows
#endif
