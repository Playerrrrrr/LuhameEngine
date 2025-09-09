#include"pch.h"
#include "Core.h"

#include "Log.h"

#include <Windows.h>

#define Luhame_BUILD_ID "v0.1a"

namespace Luhame {

	void InitializeCore()
	{
		Luhame::Log::Init();

		LH_CORE_TRACE("Luhame Engine {}", Luhame_BUILD_ID);
		LH_CORE_TRACE("Initializing...");
	}

	void ShutdownCore()
	{
		LH_CORE_TRACE("Shutting down...");
	}

}

//BOOL APIENTRY DllMain(HMODULE hModule,
//	DWORD  ul_reason_for_call,
//	LPVOID lpReserved
//)
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//		Luhame::InitializeCore();
//		break;
//	case DLL_THREAD_ATTACH:
//	case DLL_THREAD_DETACH:
//		break;
//	case DLL_PROCESS_DETACH:
//		Luhame::ShutdownCore();
//		break;
//	}
//	return TRUE;
//}