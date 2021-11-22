#ifndef __LOGGER__
#define __LOGGER__

#include <cstdio>
#include <Windows.h>
#include <typeinfo>
#include "LogManager.hpp"


class ILogger {
    public:
        ILogger(string name): m_name(name) {}
        virtual void write(const char *) = 0;

        /* identity of the logger
         * if no name, then use the class Name
         */
        virtual string GetName() {
            if (m_name.empty()) {
                return typeid(*this).name();
            } else {
                return m_name;
            }
        }

    protected:
        std::string m_name;
};


class ConsoleLogger: public ILogger {
public:
    ConsoleLogger(string name=""): ILogger(name) {
        AllocConsole();
        m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    virtual void write(const char * str) {
        WriteFile(m_consoleHandle, str, strlen(str), NULL, NULL);
    }

protected:
    HANDLE m_consoleHandle;
};


class FileLogger: public ILogger {
public:
    FileLogger(const char * filename): ILogger(filename) {
        m_file = std::fopen(filename, "w");
        printf("%b", m_file==NULL);
    }

    FileLogger(string filename): FileLogger(filename.c_str()) {}

    ~FileLogger() {
        if (m_file) {
            std::fclose(m_file);
        }
    }

    virtual void write(const char * log_string) {
        if (!m_file) {
            return;
        }
        std::fprintf(m_file, "%s", log_string);
    }

protected:
    FILE * m_file;
};


#endif //__LOGGER__