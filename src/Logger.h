#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include "Service.h"
#include "util.h"

using LOG_LEVEL = spdlog::level::level_enum;

namespace Kiwi {

class Logger : public Service {
public:
	static void Initialize();
	static void Log(const String &message, const String &logger = "default", LOG_LEVEL level = LOG_LEVEL::debug);

	const static LOG_LEVEL level = LOG_LEVEL::trace;

private:
	Logger() = delete;
	~Logger() = default;

	static std::unordered_map<String, std::shared_ptr<spdlog::logger>> logs;
};

}

#endif // __LOGGER_H__
