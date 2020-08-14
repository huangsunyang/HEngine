#pragma once
#include <string>
#include <fstream>
#include <utility>

using std::string;
using std::fstream;

class Parser {
public:
    Parser(string filename): m_stream = fstream(filename, std::ios::in) {}
    Parser * parseLine();

protected:
    fstream m_stream;
};