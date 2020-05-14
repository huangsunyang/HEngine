#ifndef __LOG_MANAGER__
#define __LOG_MANAGER__

#include <map>
#include <string>

class ILogger;

using std::map;
using std::string;

#define DEBUG(str, ...) LogManager::log(LogLevel::DEBUG, (str), __VA_ARGS__)
#define INFO(str, ...) LogManager::log(LogLevel::INFO, (str), __VA_ARGS__)
#define WARNING(str, ...) LogManager::log(LogLevel::WARNING, (str), __VA_ARGS__)
#define ERROR(str, ...) LogManager::log(LogLevel::ERROR, (str), __VA_ARGS__)

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,    
};


/*
 * LogManager manages logger, which implememnts iLogger
 * each iLogger instance implements [write] method to write
 * strings to where they want to
 */
class LogManager {
    public:
        static void init();
        static void log(int level, char * format, ...);
        static void registerLogger(ILogger * log);
        static void unregisterLogger(ILogger * log);
        static void unregisterLogger(string log_name);
        static void showAllLogger();

    protected:
        static map<string, ILogger *> m_loggers;
};


#endif // __LOG_MANAGER__
