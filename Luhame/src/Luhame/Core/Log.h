#pragma once

#include "Luhame/Core/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <spdlog/fmt/ostr.h>

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
#define LH_CORE_TRACE(...)	SPDLOG_LOGGER_TRACE(Luhame::Log::GetCoreLogger().get(),__VA_ARGS__)
#define LH_CORE_INFO(...)	SPDLOG_LOGGER_INFO (Luhame::Log::GetCoreLogger().get(),__VA_ARGS__)
#define LH_CORE_WARN(...)	SPDLOG_LOGGER_WARN (Luhame::Log::GetCoreLogger().get(),__VA_ARGS__)
#define LH_CORE_ERROR(...)	SPDLOG_LOGGER_ERROR(Luhame::Log::GetCoreLogger().get(),__VA_ARGS__)
#define LH_CORE_FATAL(...)	Luhame::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Logging Macros
#define LH_TRACE(...)	SPDLOG_LOGGER_TRACE(Luhame::Log::GetClientLogger().get(),__VA_ARGS__)
#define LH_INFO(...)	SPDLOG_LOGGER_INFO (Luhame::Log::GetClientLogger().get(),__VA_ARGS__)
#define LH_WARN(...)	SPDLOG_LOGGER_WARN (Luhame::Log::GetClientLogger().get(),__VA_ARGS__)
#define LH_ERROR(...)	SPDLOG_LOGGER_ERROR(Luhame::Log::GetClientLogger().get(),__VA_ARGS__)
#define LH_FATAL(...)	Luhame::Log::GetClientLogger()->critical(__VA_ARGS__)