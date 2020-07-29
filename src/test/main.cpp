#include "utils/FileUtils.hpp"
#include "pugixml.hpp"
#include <pybind11/embed.h>
#include <iostream>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace Utils;
using std::string;

static int test_count = 0;
static int pass_count = 0;

#define AssertEqual(a, b)               \
    do {                                \
        auto x = a;                     \
        auto y = b;                     \
        ++test_count;                   \
        if ((x) == (y)) ++pass_count;   \
        else std::cout << '[' << test_count << "] expect " << y << " but get " << x << std::endl; \
    } while (0)


#define TESTBEGIN(name)                 \
    void test##name() {


#define TESTEND(name)                       \
        printf("Test %s Finish: %d/%d pass!\n", #name, pass_count, test_count);  \
        test_count = 0;                 \
        pass_count = 0;                 \
    }

#define TEST(name)                      \
    test##name()

TESTBEGIN(FileUtils)
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
TESTEND(FileUtils)


TESTBEGIN(XmlUtils)
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("package/config/test.xml");
    std::cout << "Load result: " << result.description() << std::endl;
    AssertEqual(doc.child("mesh").attribute("name").value(), string("mesh_root"));
    pugi::xml_node mesh = doc.child("mesh");
    int index = 1;
    for (pugi::xml_node node = mesh.first_child(); node; node = node.next_sibling(), index++) {
        AssertEqual(node.attribute("attr").value(), string("value") + std::to_string(index));
    }
    AssertEqual(doc.child("mesh").child("node").attribute("attr").value(), string("value1"));
TESTEND(XmlUtils)


TESTBEGIN(Pybind11)
    namespace py = pybind11;
    py::scoped_interpreter p;
    py::list sys_path = py::module::import("sys").attr("path");
    for (auto path: sys_path) {
        py::print(path);
    }
TESTEND(Pybind11)


TESTBEGIN(Freetype)
    FT_Library ft;
    FT_Face face;
TESTEND(FreeType)


int main() {
    TEST(FileUtils);
    TEST(XmlUtils);
    TEST(Pybind11);

    getchar();
    return 0;
}