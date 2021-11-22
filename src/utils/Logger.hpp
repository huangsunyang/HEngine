#ifndef __LOGGER__
#define __LOGGER__

#include <cstdio>
#include <typeinfo>
#include <Windows.h>
#include "LogManager.hpp"


class ILogger {
    public:
        explicit ILogger(const string& name);
        virtual void write(const char *) = 0;

        /* identity of the logger
         * if no name, then use the class Name
         */
        virtual string GetName();

    protected:
        std::string m_name;
};


class ConsoleLogger: public ILogger {
public:
    explicit ConsoleLogger(const string& name="");
    void write(const char * str) override;

protected:
    HANDLE m_consoleHandle;
};


class FileLogger: public ILogger {
public:
    explicit FileLogger(const char * filename);
    explicit FileLogger(const string& filename);
    ~FileLogger();

    void write(const char * log_string) override;

protected:
    FILE * m_file;
};


#endif //__LOGGER__