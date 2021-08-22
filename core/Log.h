
#ifndef LOG_H
#define LOG_H

// #include "dypch.h" // at some point I might create a PCH, if I do, probably it should be as early as possible
#include <core/Core.h>
#include <vendor/spdlog/include/spdlog/spdlog.h>
#include <vendor/spdlog/include/spdlog/fmt/ostr.h>

class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;


};

// Core Log macros
#define LOG_CORE_TRACE(...) Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)  Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)  Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_FATAL(...) Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Log macros
#define LOG_TRACE(...) Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)  Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...) Log::GetClientLogger()->fatal(__VA_ARGS__)


#endif // LOG_H
