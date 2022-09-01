#include "pLogger.h"

std::shared_ptr<spdlog::logger> pLogger::m_logger;

void pLogger::init()
{
	// Set logger pattern >> [PSYCHO_PHYSICS]
	//spdlog::set_pattern("%^[%T] %n:  %v%$");
	spdlog::set_pattern("[%n]:  %v%$");

	m_logger = spdlog::stdout_color_mt("PSYCHO_PHYSICS");
	m_logger->set_level(spdlog::level::trace);
}