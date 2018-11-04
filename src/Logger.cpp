#include "Logger.h"
#include <iostream>
namespace Kiwi {

bool Logger::initialized(false);
std::unordered_map<String, std::shared_ptr<spdlog::logger>> Logger::logs;

void Logger::Initialize() {
	spdlog::set_pattern("[%H:%M:%S.%e] [%l] %v");
	try {
		logs.emplace("default", spdlog::basic_logger_mt("default", "log/log.txt"));
		logs.at("default")->set_level(Logger::level);
		Logger::initialized = true;
		logs.at("default")->set_pattern("[%c] [%l] %v");
		logs.at("default")->log(Logger::level, "Logger initialized for this session");
		logs.at("default")->set_pattern("[%H:%M:%S.%e] [%l] %v");
	} catch (spdlog::spdlog_ex &e) {
		std::cout << "Error initializing logger: " << e.what() << std::endl <<
			"Logging will be disabled for this session" << std::endl;
		Logger::initialized = false;
	}
}

void Logger::Log(const String &message, LOG_LEVEL level) {
	if (!Logger::initialized) { return; }
	if (level >= Logger::level) {
		auto def_log = logs.at("default");
		if (def_log != log) {
			logs.at("default")->log(level, "[" + log->name() + "] " + message);
		}
		log->log(level, message);
	}
}

}