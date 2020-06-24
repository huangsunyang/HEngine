#pragma once

#include <string>
#include <vector>

using std::vector;
using std::string;

namespace Utils {

bool string_starts_with(string str, string prefix);
vector<string> string_split(string str, string split);
string string_lstrip(string str, string strip);
string string_rstrip(string str, string strip);
void string_rstrip_inplace(string& str, string strip);
string string_strip(string str, string strip);
string string_replace(string str, string old_part, string new_part);

};