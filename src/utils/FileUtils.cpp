#include "utils/FileUtils.hpp"
#include "utils/StringUtils.hpp"

namespace Utils {

string file_format(string filename) {
    size_t filename_length = filename.size();
    auto dot_pos = filename.find_last_of('.');
    return dot_pos != string::npos ? filename.substr(dot_pos) : "";
}


string file_dirname(string filename) {
    auto slash_pos = filename.find_last_of('/');
    return slash_pos != string::npos ? filename.substr(0, slash_pos) : "./";
}


string file_basename(string filename) {
    auto slash_pos = filename.find_last_of('/');
    return slash_pos != string::npos ? filename.substr(slash_pos + 1) : filename;
}


void _path_up_dir(string& normed_dir) {
    if (normed_dir.empty()) {
        normed_dir = "..";
        return;
    }

    auto last_slash_pos = normed_dir.find_last_of('/');
    if (last_slash_pos == string::npos) {
        if (normed_dir == "..") normed_dir = "../..";
        else normed_dir.clear();
    } else if (last_slash_pos == 0) {
        normed_dir.resize(1);
    } else if (normed_dir.substr(last_slash_pos + 1) == "..") {
        normed_dir += "/..";
    } else {
        normed_dir.resize(last_slash_pos);
    }
}

void _path_down_dir(string& normed_dir, string folder) {
    if (!normed_dir.empty() && normed_dir.back() != '/') normed_dir.push_back('/');
    normed_dir += folder;
}


string path_join(vector<string> paths) {
    string joined_path = "";
    for (auto path: paths) {
        path = path_norm(path);
        if (string_starts_with(path, "/")) {
            joined_path = path;
        } else {
            string_rstrip_inplace(joined_path, "/");
            _path_down_dir(joined_path, path);
        }
    }
    return joined_path;
}


string path_norm(string filename) {
    filename = string_replace(filename, "\\", "/");
    vector<string> parts = string_split(filename, "/");
    string ret = "";
    for (int i = 0; i < parts.size(); i++) {
        printf("%d %s\n", i, parts[i].c_str());
        if (parts[i] == ".") {
            continue;
        } else if (parts[i] == "") {
            if (i == 0) {
                ret += "/";
            } else continue;
        } else if (parts[i] == "..") {
            _path_up_dir(ret); 
        } else {
            _path_down_dir(ret, parts[i]);
        }
    }
    return ret.empty() ? "." : ret;
}


string path_ensure_dir(string filename, string dirname) {
    filename = path_norm(filename);
    dirname = path_norm(dirname);
    if (!string_starts_with(filename, dirname)) {
        return path_join({dirname, filename});
    }
    return filename;
}

}