#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class pLogger
{
public:
	static void init();

	inline static std::shared_ptr<spdlog::logger> get_logger() { return m_logger; }
private:
	static std::shared_ptr<spdlog::logger> m_logger;
};


#define PSYCHO_INFO(...)      pLogger::get_logger()->info(__VA_ARGS__)
#define PSYCHO_DEBUG(...)     pLogger::get_logger()->debug(__VA_ARGS__)
#define PSYCHO_TRACE(...)     pLogger::get_logger()->trace(__VA_ARGS__)
#define PSYCHO_WARN(...)      pLogger::get_logger()->warn(__VA_ARGS__)
#define PSYCHO_ERROR(...)     pLogger::get_logger()->error(__VA_ARGS__)
#define PSYCHO_CRITICAL(...)  pLogger::get_logger()->critical(__VA_ARGS__)