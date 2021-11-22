#include "utils/Logger.hpp"

ILogger::ILogger(const string& name): m_name(name) {}

string ILogger::GetName() {
    if (m_name.empty()) {
        return typeid(*this).name();
    } else {
        return m_name;
    }
}

ConsoleLogger::ConsoleLogger(const string& name): ILogger(name) {
    AllocConsole();
    m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void ConsoleLogger::write(const char *str) {
    WriteFile(m_consoleHandle, str, strlen(str), NULL, NULL);
}

FileLogger::FileLogger(const char *filename): ILogger(filename) {
    m_file = std::fopen(filename, "w");
}

FileLogger::FileLogger(const string& filename): FileLogger(filename.c_str()) {

}

FileLogger::~FileLogger() noexcept {
    if (m_file) {
        std::fclose(m_file);
    }
}

void FileLogger::write(const char *log_string) {
    if (!m_file) {
        return;
    }
    std::fprintf(m_file, "%s", log_string);
}
