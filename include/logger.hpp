#ifndef __LOGGER__
#define __LOGGER__
#include <fstream>

#define LOG(str) Logger::get_instance()->log(str)
using std::fstream;

class Logger {
public:
    Logger() {
        f.open("log/log.txt", fstream::out);
        f.close();
    }

    static Logger * get_instance() {
        static Logger * instance = new Logger;
        return instance;
    }

    template <typename T>
    void log(T str) {
        f.open("log/log.txt", fstream::app);
        f << str;
        f.close();
    }

    ~Logger() {
        f.close();
    }

    std::fstream f;
};

#endif
