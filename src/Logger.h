#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "util.h"

using LOG_LEVEL = spdlog::level::level_enum;

namespace Kiwi {

class ServiceLocator;

class Logger {
public:
	explicit Logger(std::shared_ptr<spdlog::logger> &log) : log(log) {}
	virtual void Log(const String &message, LOG_LEVEL level = LOG_LEVEL::debug);

	const static LOG_LEVEL level = LOG_LEVEL::trace;

private:
	friend ServiceLocator;

	Logger() : log() {}

	static void Initialize();
	static bool initialized;

	std::shared_ptr<spdlog::logger> log;

	static std::unordered_map<String, std::shared_ptr<spdlog::logger>> logs;
};

#define LOG_DEFAULT   "default"
#define LOG_RENDERING "rendering"
#define LOG_PHYSICS   "physics"

}

#endif // __LOGGER_H__
