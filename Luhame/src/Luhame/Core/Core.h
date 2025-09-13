#pragma once
namespace Luhame {

	void InitializeCore();
	void ShutdownCore();

}

//#ifdef LUHAME_BUILD_DLL
//	#define LUHAME_API __declspec(dllexport)
//#else
//	#define LUHAME_API __declspec(dllimport)
//#endif


#ifdef LH_DEBUG
	#define LH_ENABLE_ASSERTS 
#endif 

#ifdef LH_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	#ifdef LH_BUILD_DLL
		#define LUHAME_API __declspec(dllexport)
	#else
		#define LUHAME_API __declspec(dllimport)
	#endif
#else		
	#define LUHAME_API 
#endif
#else
	#error Luhame only supports Windows!
#endif

#ifdef LH_ENABLE_ASSERTS
	#define LH_ASSERT(x, ...) { if(!(x)) { LH_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define LH_CORE_ASSERT(x, ...) { if(!(x)) { LH_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define LH_ASSERT(x, ...)
	#define LH_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)


#define LH_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)

constexpr bool LH_RENDERER_DEBUG = true;

