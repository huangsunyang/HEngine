#ifndef __FILE_UTILS__
#define __FILE_UTILS__

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace Utils {

string file_format(string filename);
string path_ensure_dir(string file, string dir);

}

#endif