#include "Logger.h"
#include <iostream>
namespace Kiwi {

std::unordered_map<String, std::shared_ptr<spdlog::logger>> Logger::logs;
void Logger::Initialize() {
	spdlog::set_pattern("[%H:%M:%S.%e] [%l] %v");
	logs.emplace("default", spdlog::basic_logger_mt("default", "log/log.txt"));
	logs.at("default")->set_level(Logger::level);
	logs.at("default")->set_pattern("[%H:%M:%S.%e] [%l] %v");
	Log("Default logger at log.txt initialized");
}

void Logger::Log(const String &message, const String &logger, LOG_LEVEL level) {
	std::shared_ptr<spdlog::logger> log;
	try {
		log = logs.at(logger);
	} catch (std::exception &e) {
		logs.emplace(logger, spdlog::basic_logger_mt(logger, "log/" + logger + ".txt"));
		log = logs.at(logger);
		log->set_level(Logger::level);
	}
	if (level >= Logger::level) {
		auto def_log = logs.at("default");
		if (def_log != log) {
			logs.at("default")->log(level, "[" + logger + "] " + message);
		}
		log->log(level, message);
	}
}

}