#include "utils/StringUtils.hpp"

namespace Utils {

bool string_starts_with(string str, string prefix) {
    return str.rfind(prefix, 0) == 0;
}

vector<string> string_space_split(string str) {
    vector<string> ret;

    for (auto i = 0; i < str.size(); i++) {
        auto j = i;
        for (; j < str.size() && str[j] != ' '; j++) {
        }
        if (j > i) {
            ret.push_back(str.substr(i, j - i));
            i = j;
        }
    }

    return ret;
}

vector<string> string_split(string str, string split) {
    auto split_size = split.size();
    auto search_start = 0;
    vector<string> ret;

    while (true) {
        auto found_pos = str.find(split, search_start);
        if (found_pos == string::npos) {
            ret.push_back(str.substr(search_start));
            break;
        } else {
            ret.push_back(str.substr(search_start, found_pos - search_start));
            search_start = found_pos + split_size;        
        }
    }

    return ret;
}


string string_lstrip(string str, string strip) {
    auto search_start = 0;
    
    while (search_start != string::npos) {
        auto found_pos = str.find(strip, search_start);
        if (found_pos != search_start) break;
        search_start += strip.size();
    }
    return str.substr(search_start);
}


string string_rstrip(string str, string strip) {
    auto search_start = str.size() - strip.size();
    if (search_start < 0) {
        return str;
    }
    
    while (search_start != string::npos) {
        auto found_pos = str.rfind(strip, search_start);
        if (found_pos != search_start) break;
        search_start -= strip.size();
    }
    return str.substr(0, search_start + strip.size());
}


void string_rstrip_inplace(string& str, string strip) {
    auto search_start = str.size() - strip.size();
    if (search_start < 0) {
        return;
    }
    
    while (search_start != string::npos) {
        auto found_pos = str.rfind(strip, search_start);
        if (found_pos != search_start) break;
        search_start -= strip.size();
    }
    str.resize(search_start + strip.size());
}


string string_strip(string str, string strip) {
    str = string_lstrip(str, strip);
    return string_rstrip(str, strip);
}


string string_replace(string str, string old_part, string new_part) {
    auto search_start = 0;
    string ret = "";
    while (true) {
        auto found_pos = str.find(old_part, search_start);
        if (found_pos == string::npos) {
            ret += str.substr(search_start);
            break;
        } else {
            ret += str.substr(search_start, found_pos - search_start);
            ret += new_part;
            search_start = found_pos + old_part.size();
        }
    }
    return ret;
}

}