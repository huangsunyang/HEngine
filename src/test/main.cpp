#include "utils/FileUtils.hpp"
#include <iostream>

using namespace Utils;

static int test_count = 0;
static int pass_count = 0;

#define AssertEqual(a, b)               \
    do {                                \
        auto x = a;                     \
        auto y = b;                     \
        if ((x) == (y)) ++pass_count;   \
        else std::cout << test_count << ". expect " << y << " but get " << x << std::endl; \
        ++test_count;                   \
    } while (0)

int main() {
    AssertEqual(path_norm("a\\b\\c"), "a/b/c");
    AssertEqual(path_norm("a//b//c"), "a/b/c");
    AssertEqual(path_norm("./a/./b/./c"), "a/b/c");
    AssertEqual(path_norm("a/../b/../c"), "c");
    AssertEqual(path_norm("a/../b/.."), ".");
    AssertEqual(path_norm("./../"), "..");
    AssertEqual(path_norm("./."), ".");
    AssertEqual(path_norm("../b/../.."), "../..");
    AssertEqual(path_norm("/../a/b/c"), "/a/b/c");

    AssertEqual(path_ensure_dir("./a/b", "res/Package"), "res/Package/a/b");
    AssertEqual(path_ensure_dir("./res/Package/a/b", "res/Package"), "res/Package/a/b");
    AssertEqual(path_ensure_dir("a/b", "."), "a/b");
    AssertEqual(path_ensure_dir("../a/b", ".."), "../a/b");
    AssertEqual(path_ensure_dir("a/b", ".."), "../a/b");

    printf("Test Finish: %d/%d pass!\n", pass_count, test_count);
    getchar();
    return 0;
}