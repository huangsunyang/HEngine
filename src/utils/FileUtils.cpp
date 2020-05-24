#include "utils/FileUtils.hpp"

using namespace std;

namespace Utils {

string file_format(string filename) {
    size_t filename_length = filename.size();
    int dot_pos = filename.find_last_of('.');
    return dot_pos>=0 ? filename.substr(dot_pos) : "";
}

}