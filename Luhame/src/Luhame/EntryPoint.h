#pragma once

#ifdef LH_PLATFORM_WINDOWS
//.h文件被唯一引用时不会编译原文件不会被编译？
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
