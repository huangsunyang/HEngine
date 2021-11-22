#ifndef __FILE_UTILS__
#define __FILE_UTILS__

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace Utils {

string file_format(string filename);
string file_dirname(string filename);
string file_basename(string filename);
string path_norm(string filename);
string path_join(vector<string> paths);
string path_ensure_dir(string file, string dir);

}

#endif