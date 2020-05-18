#ifndef __LOG_MANAGER__
#define __LOG_MANAGER__

#include <map>
#include <string>
#include <Python.h>

class ILogger;

using std::map;
using std::string;

#ifdef ERROR
#undef ERROR    // conflict with other macro
#endif

#define DEBUG(str, ...) LOG::LogManager::log(LOG::LogLevel::DEBUG, (str), __VA_ARGS__)
#define INFO(str, ...) LOG::LogManager::log(LOG::LogLevel::INFO, (str), __VA_ARGS__)
#define WARNING(str, ...) LOG::LogManager::log(LOG::LogLevel::WARNING, (str), __VA_ARGS__)
#define ERRORS(str, ...) LOG::LogManager::log(LOG::LogLevel::ERROR, (str), __VA_ARGS__)


namespace LOG {

enum class LogLevel: unsigned int {
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
        static void log(LogLevel level, char * format, ...);
        static void registerLogger(ILogger * log);
        static void unregisterLogger(ILogger * log);
        static void unregisterLogger(string log_name);
        static void showAllLogger();

    protected:
        static map<string, ILogger *> m_loggers;
};


/* python LogManager wrapper，which redirect python print to
 * LogManager::log
 */
struct PyLogManager
{
    PyObject_HEAD
};

extern PyTypeObject PyLogManagerType;

}

#endif // __LOG_MANAGER__
