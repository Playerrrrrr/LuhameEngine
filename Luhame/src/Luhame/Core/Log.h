#pragma once

#include "Luhame/Core/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Luhame {

	class LUHAME_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core Logging Macros
#define LH_CORE_TRACE(...)	Luhame::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LH_CORE_INFO(...)	Luhame::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LH_CORE_WARN(...)	Luhame::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LH_CORE_ERROR(...)	Luhame::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LH_CORE_FATAL(...)	Luhame::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Logging Macros
#define LH_TRACE(...)	Luhame::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LH_INFO(...)	Luhame::Log::GetClientLogger()->info(__VA_ARGS__)
#define LH_WARN(...)	Luhame::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LH_ERROR(...)	Luhame::Log::GetClientLogger()->error(__VA_ARGS__)
#define LH_FATAL(...)	Luhame::Log::GetClientLogger()->critical(__VA_ARGS__)