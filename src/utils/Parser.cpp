#include "Utils/Parser.hpp"

Parser * Parser::parseLine() {
    string line;
    std::getline(*m_istream, line);
    auto s = new stringstream(line);
    return new Parser(s);
}

