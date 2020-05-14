#include "LogManager.hpp"
#include "Logger.hpp"
#include <cstdarg>

map<string, ILogger *> LogManager::m_loggers = map<string, ILogger *>();

void LogManager::init() {
    LogManager::registerLogger(new ConsoleLogger);
    LogManager::registerLogger(new FileLogger("log.txt"));
}

void LogManager::log(int level, char * format, ...) {
    va_list args, args1;
    va_start(args, format);
    va_copy(args1, args);

    size_t len = _vscprintf(format, args) + 1;
    char * log_string = new char[len];
    vsprintf(log_string, format, args1);
    for(auto iter: LogManager::m_loggers) {
        iter.second->write(log_string);
    }
    delete log_string;

    va_end(args);
}

void LogManager::registerLogger(ILogger * logger) {
    LogManager::m_loggers[logger->GetName()] = logger;
}

void LogManager::unregisterLogger(ILogger * logger) {
    LogManager::unregisterLogger(logger->GetName());
}

void LogManager::unregisterLogger(std::string name) {
    LogManager::m_loggers.erase(name);
}

void LogManager::showAllLogger() {
    for (auto iter: LogManager::m_loggers) {
        LogManager::log(LogLevel::INFO, "%s\n", iter.first.c_str());
    }
}
