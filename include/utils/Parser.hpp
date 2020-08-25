#pragma once
#include <string>
#include <istream>
#include <sstream> 
#include <fstream>
#include <utility>

using std::string;
using std::istream;
using std::fstream;
using std::stringstream;

class Parser {
public:
    Parser() = delete;
    Parser(istream& s): m_cursor(0) { m_istream = &s; }
    Parser(istream * s): m_cursor(0) { m_istream = s; }
    Parser(string& s): m_cursor(0) { m_istream = new stringstream(s); }
    
    Parser * parseLine();

    template <typename T, typename... Q>
    bool parse(T& ret, Q&... other) {
        if (!parse(ret)) {
            return false;
        }
        return parse(other...);
    }

    template <typename T, typename... Q>
    bool parse(T& ret) {
        if (m_istream->eof()) {
            return false;
        }
        *m_istream >> ret;
        return true;
    }

    void skip(int n) {
        string ret;
        while (n--) {
            if (m_istream->eof()) {
                return;
            }
            *m_istream >> ret;
        }
    }

    template <typename T>
    T parse() {
        T ret;
        *m_istream >> ret;
        return ret;
    }

protected:
    void skipSpace();

protected:
    istream * m_istream;
    size_t m_cursor;
};