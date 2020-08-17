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
    void parse(T& ret, Q&... other) {
        *m_istream >> ret;
        parser(other...);
    }

    template <typename T, typename... Q>
    void parse(T& ret) {
        *m_istream >> ret;
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